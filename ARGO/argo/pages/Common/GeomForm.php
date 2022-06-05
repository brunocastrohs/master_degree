<?require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");?>
<!DOCTYPE html/>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
</head>
<body>
<div id='commonForm'>
<form id='geomForm'>

		<div id="coordinate0">

		<div class="label">
			Long.(X): <input id="x0" class="coordinateValueField" onkeyup="GeomManager.isInteger(x0,this);" type="text"/>
		</div>
		
		<div class="label">
			Latit.(Y): <input id="y0" class="coordinateValueField" onkeyup="GeomManager.isInteger(y0,this);" type="text"/>
		</div>

		</div>
		
		<div id="addedCoordinateList" ></div>
		
		<div id="addButton" onclick="GeomManager.addCoordinate();">Adicionar Coordenadas</div>

</form>
</div>
</body>
</html>