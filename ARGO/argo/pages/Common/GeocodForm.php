<div id='commonForm'>
<form id='geocodForm'>
	<div style="width: 300px; padding-left: 20px;">
	
		<div class="label">
			Endereço: <input id="filterValueField1000" class="filterValueField" onkeyup="MapAction.autoComplete('1000',this.value);" type="text"/>
		</div>
		<div class='autocomplete' id='autocomplete1000'></div>
		<span class='at' id='at1000'><img src='/ARGO/images/autocomplete.gif' alt='loading' /></span>
		<div class='sample' id='sample1000'>text</div>
		<input class="search" id="geocodButton" type="button" onclick="MapAction.geocode('filterValueField1000');" value="Encontre" />
	
	</div>
</form>
</div>