<?php header("Content-Type: text/html; charset=UTF-8")?>
<!DOCTYPE html/>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
</head>
<body>
	<div id='commonForm'>
		<form id='queryGISForm'>
			<div style="width: 200px; padding-left: 20px;">

				<div class="label">
					Camada <select id="layerSelector"
						onclick='MapAction.populateLayerSelector();'
						onchange="MapAction.populateLayerAttributes();MapAction.layerChange(this.value);"></select>
				</div>

				<div id="attribute0">
					<div class="label">
						Atributos <select id="layerAttributes0" class="layerAttributes"
							onchange="MapAction.layerAttributesChange(0);"></select>
					</div>

					<div class="label">
						Valor <input id="filterValueField0" class="filterValueField"
							onkeyup="MapAction.isInteger(0,this);" type="text" />
					</div>
					<div class='sample' id='sample0'>Tipo:</div>
				</div>

				<div id="addedAttributeList"></div>

				<div id="addButton" onclick="MapAction.addAttribute();">Adicionar
					Atributo</div>

				<input class="remove" id="resetButton" type="button"
					onclick="MapAction.clearFilter();" value="Limpar" /> <input
					class="search" id="filterButton" type="button"
					onclick="MapAction.enableFilter();" value="Pesquisar" /> <span
					id="pr"><img src="/GeoCod/images/progressbar.png" alt="loading" />
				</span>

				<div id="showButton" onclick="MapAction.retrieveRecords();">Visualizar
					Registros</div>
				<span id="pr"><img src="/GeoCod/images/progressbar.gif"
					alt="loading" /> </span>

				<div id="records"></div>

			</div>
		</form>
	</div>
</body>
</html>
