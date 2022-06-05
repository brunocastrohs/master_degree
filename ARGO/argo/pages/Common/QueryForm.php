<?require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");?>
<div id='commonForm'>
<form id='searchForm'>
	<div style="width: 200px; padding-left: 0px;">
		<div id="attribute0">
		<div class="label">
			Atributos 
			<select id="layerAttributes0" class="layerAttributes" onchange="FormManager.layerAttributesChange(0);" >
			<?php 
				if($_GET && SecurityManager::checkSession()){
					$clauses = "and (format_type(pg_attribute.atttypid, pg_attribute.atttypmod) like '%character%' or format_type(pg_attribute.atttypid, pg_attribute.atttypmod) like '%text%')";
					echo GenericBean::retrieveColumnsOptions(base64_decode($_GET['x']),base64_decode($_GET['y']),$clauses);
				}
				else if($_GET && !SecurityManager::checkSession()){
					$clauses = "and pg_attribute.attname = 'nome'";
					echo GenericBean::retrieveColumnsOptions(base64_decode($_GET['x']),base64_decode($_GET['y']),$clauses);
				}
			?>
			</select>
		</div>

		<div class="label">
			Valor <input id="filterValueField0" class="filterValueField" onkeyup="FormManager.isInteger(0,this);" type="text"/>
		</div>
		<div class='sample' id='sample0'>Tipo:</div>
		</div>
		
		<div id="addedAttributeList" ></div>
		
		<div id="addButton" onclick="FormManager.addAttribute();">Adicionar Atributo</div>
		
		<input class="search" id="filterButton" type="button" onclick="FormManager.enableFilter();" value="Pesquisar" /> 
		<span id="pr"><img src="/ARGO/images/preloader.gif" alt="loading" /></span>
		
		<div id="records"></div>

	</div>
</form>
</div>