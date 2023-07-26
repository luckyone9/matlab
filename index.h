const char web_content[] PROGMEM = R"=====(
<html>
<head>
	<meta charset="utf-8">
	<title>ĐIỀU KHIỂN PID CÂN BẰNG BỒN NƯỚC ĐƠN</title>
	<style>
		
		
		.mytab{
			width:100%;
			height:auto;
			text-align: center;
		}
		.right{
			text-align:right;
		}
		
		.loadbar
		{
			 width:94%;
			 height:300px;
			 background-color:#fff;
			 border:5px solid #C71585;
			 position:relative; 
		}
		.bar
		{
			width:100%;
			display:block;        
			font-family:arial;
			font-size:12px; 
			background-color:#0000ff;
			color:#fff;       
			position:absolute;
			bottom:0;        
		}
				
	</style>
	<script>
		function loadXmlHttp() {
			if (window.XMLHttpRequest){
				xh = new XMLHttpRequest();
			}else if (window.ActiveXObject){
				try{
					xh = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch(e){}
			}
		}
	</script>
</head>
<body onload = 'initLoad()'>

<table border="0" cellpadding="0" style="height: 60px; width: 100%;">
		<tr>
			<td style="width: 100px;" onclick="location.reload(true);">
				<img id = "logo" alt="Logo IOT"  style="cursor: default; width: 100px; height: 60px;"></img>
			</td>
			<td>
			<center><b style="color:rgb(204, 68, 0);font-size:22px">ESP32 ĐIỀU KHIỂN PID CÂN BẰNG BỒN NƯỚC ĐƠN</b></center>
			</td>
		</tr>
</table><br>


	
<!--table 1: Hệ thống tưới-->
	<table width = "100%" border = 0>
		<tr>
			<td><b><i style="color:blue;">HỆ BỒN NƯỚC ĐƠN</i></b></td>
			<td style = "text-align :right;">
				<a href = "/para" onclick = "StopPID();">View Setting Page</a>
			</td>
		</tr>
	</table>
	<!-- Bồn nước Đơn -->
	<table class = "mytab" border="1" cellpadding="0" >
		<tr>
			<td style='width:40%;'>
				<div class="loadbar" >
				<strong class="bar" id = "pump" style='height:10%;'>10%</strong>
				</div>
			</td>
			
			<td style = 'background-color:#F0FFFF'>
				<table width = "100%" border = 1>
					<tr>
						<td>
							<b>Water</b>
						</td>
						<td>
							<label id = 'WL' >0 cm</label>
						</td>
					</tr>
					<tr>
						<td>
							<b>Setpoint</b>
						</td>
						<td>
							<label id = 'SP' >0 cm</label>
						</td>
					</tr>
					<tr>
						<td>
							<b>KP</b>
						</td>
						<td>
							<label id = 'KP' >0</label>
						</td>
					</tr>
					<tr>
						<td>
							<b>KI</b>
						</td>
						<td>
							<label id = 'KI' >0</label>
						</td>
					</tr>
					<tr>
						<td>
							<b>KD</b>
						</td>
						<td>
							<label id = 'KD' >0</label>
						</td>
					</tr>
										
				</table>
				
				<button  style="height:60px; width:100%;" id = "StartPID" onclick = "StartPID();" >START&nbsp;PID</button>
				<button  style="height:60px; width:100%;" id = "StopPID" onclick = "StopPID();" >STOP&nbsp;PID</button>
				
				
			</td>
			

	</table>

	

	<script>
		var pids = 0;
		var ColorOn = "#00FF40";
		var ColorOff = "";//"#C0C0C0";
		var ColorStop = "#DC143C";
		//cp = 0 -> rcp = 0; cp = 40 -> rcp = 100
		// cp = a(cm) -> rcp =  a*100/40;
		var rlp = 0; // low pum rate%
		var rhp = 100; // high pum rate%
		
		var lp = 0; // low pum rate%
		var hp = 40; // high pum rate%
		var wlmax = 40;
		var wl = 5; // current pum cm
		var wlr = wl*100/wlmax; // current pum rate%
		
		var sp = 20; //setpoint 
		var kp = 0;
		var ki = 0;
		var kd = 0;
		
		var xh;
		var run = 0;
		var runDev = 0;
		
		function UpdateColorButton(){
			bt_StartPID = document.getElementById("StartPID");
			bt_StopPID = document.getElementById("StopPID");
			bt_StartPID.style.backgroundColor = (pids == 1)?ColorOn:ColorOff;
			bt_StopPID.style.backgroundColor = (pids == 0)?ColorStop:ColorOff;
		}
		function UpdateSomeThing(){
			//update pum level 
			document.getElementById('WL').innerText = wl + 'cm';
			document.getElementById('SP').innerText = sp + 'cm';
			document.getElementById('KP').innerText = kp + '..';
			document.getElementById('KI').innerText = ki + '..';
			document.getElementById('KD').innerText = kd + '..';
			var pump = document.getElementById('pump');
			pump.style.height = wlr+'%';
			pump.innerText = wlr+'%';
		}
		
		function StartPID(){
			loadXmlHttp();
			if(xh){
				xh.open("GET","/PID1",true);
				// For onreadystatechange
				xh.onreadystatechange = function(){
					if (xh.readyState == 4){
						if(xh.status == 200) {
							pids = Number(xh.responseText);
							//var res = JSON.parse(xh.responseText);
							//pids = Number(res.pids);
							UpdateColorButton();
							//Ndev = Number(res.ndev); //Ndev
							
						}
					}
				}
				}
				xh.send(null);	
		}
		function StopPID(){
			loadXmlHttp();
			if(xh){
				xh.open("GET","/PID0",true);
				// For onreadystatechange
				xh.onreadystatechange = function(){
					if (xh.readyState == 4){
						if(xh.status == 200) {
							pids = Number(xh.responseText);
							//var res = JSON.parse(xh.responseText);
							//pids = Number(res.pids);
							UpdateColorButton();
							//Ndev = Number(res.ndev); //Ndev
							
						}
					}
				}
				}
				xh.send(null);	
		}	
		function initLoad(){
			loadXmlHttp();
			if(xh){
				xh.open("GET","/init",true);
				// For onreadystatechange
				xh.onreadystatechange = function(){
					if (xh.readyState == 4){
						if(xh.status == 200) {
							var res = JSON.parse(xh.responseText);
							pids = Number(res.runPID);
							wl = Number(res.WL);
							sp = Number(res.SP);
							kp = Number(res.Kp);
							ki = Number(res.Ki);
							kd = Number(res.Kd);
							wlr = wl*100/wlmax;
							UpdateColorButton();
							UpdateSomeThing();
							//Ndev = Number(res.ndev); //Ndev
							
						}
					}
				}
				}
			xh.send(null);	
		}	
		
		function UpdatePum(){
			if(pids==1){
				loadXmlHttp();
				if(xh){
					xh.open("GET","/WLN",true);	// Water Level Now
					// For onreadystatechange
					xh.onreadystatechange = function(){
						if (xh.readyState == 4){
							if(xh.status == 200) {
								wl = Number(xh.responseText);
								wlr = wl*100/wlmax;
								var pump = document.getElementById('pump');
								pump.style.height = wlr+'%';
								pump.innerText = wlr+'%';
								document.getElementById('WL').innerText = wl + 'cm';
							}
						}
					}
					}
				xh.send(null);
			}
			
		}

		
		var  UpdatePumState = setInterval(UpdatePum, 2077);//3000 run OK
		
	</script>
</body>
</html>
)=====";
