<?php header("Content-Type: text/html; charset=UTF-8")?>
<!DOCTYPE html/>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
</head>
<body>
<div id='commonForm'>
<form id='queryGISForm'>
	<div style="width: 300; padding-left: 20px;">

		<div class="label">
			Camada a ser Pesquisada:  <select id="layerSelector" onclick='MapAction.populateLayerSelector();' onchange="MapAction.populateLayerAttributes();MapAction.layerChange(this.value);"></select>
		</div>

		<div id="attribute0">
		<div class="label">
			Atributos: <select id="layerAttributes0" class="layerAttributes" onchange="MapAction.layerAttributesChange(0);" ></select>
		</div>

		<div class="label">
			Valor: <input id="filterValueField0" class="filterValueField" onkeyup="MapAction.isInteger(0,this);MapAction.autoComplete(0,this.value);" type="text"/>
		</div>
		<div class='autocomplete' id='autocomplete0'></div>
		<span class='at' id='at0'><img src='/ARGO/images/autocomplete.gif' alt='loading' /></span>
		<div class='sample' id='sample0'></div>
		</div>
		
		<div id="addedAttributeList" ></div>
		
		<div id="addButton" onclick="MapAction.addAttribute();">Adicionar Atributo</div>
		
		<input class="report" id="filterButton" type="button" onclick="MapAction.openColumnScreen()" value="Exportar" />
		<input class="erase" id="resetButton" type="button"  onclick="MapAction.clearFilter();" 	value="Limpar" /> 
		<input class="search" id="filterButton" type="button" onclick="MapAction.enableFilter();" value="Pesquisar" />
		<br/>
		<div id="applyFilter">Apenas Destacar<input id='applyFilterBox' type='checkbox' onclick="MapAction.clearFilter();" /></div>
		
		<div id="showButton" onclick="MapAction.retrieveRecords();">Visualizar Registros</div>

		
		<div id="records"></div>

	</div>
</form>
</div>
</body>
</html>
