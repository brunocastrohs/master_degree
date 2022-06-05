//The Filter Hour
FormManager = {

x:'',
y:'',
z:'',
o:'',
u:'',
fs:'',
id:'',
attCount: 0,
data:{},
comboHolder:[],
defaultCombo:'',
defaultComboValue:'',
defaultComboTextValue:'',
fkData:[],
kumeDays:[],
kumeDaysRestriction:[],
hasSubKume:false,
kume:null,
subkume:null,

//atualiza o informativo do tipo abaixo do input
layerAttributesChange : function(index){
	 
	var selectedOption = document.getElementById('layerAttributes'+index);
	
	var layerType = selectedOption.options[selectedOption.selectedIndex].id;
	
	document.getElementById('sample'+index).innerHTML = "Tipo: "+layerType;
	
},

//mascara para numericos
isInteger : function(index,campo){
	
	var selectedOption = document.getElementById('layerAttributes'+index);
	
	var layerType = selectedOption.options[selectedOption.selectedIndex].id;
	
	if(layerType.search("text") != -1 && layerType.search("character") != -1){
	
	var digits="0123456789." ; 
	var campo_temp;   
	    for (var i=0; i < campo.value.length; i++){  
	        campo_temp=campo.value.substring(i,i+1);   
	        if (digits.indexOf(campo_temp)==-1){  
	            campo.value = campo.value.substring(0,i);  
	        }  
	    }  
	}  
},

createAttribute : function(index){
	
	var firstContent  = document.getElementById('layerAttributes0').innerHTML;
	
	return '<div id="attribute'+index+'"><div class="label">Atributos <select id="layerAttributes'+index+'" class="layerAttributes" onchange="FormManager.layerAttributesChange('+index+');">'+firstContent+'</select></div>'+
		   '<div class="label">Valor <input id="filterValueField'+index+'" class="filterValueField" type="text" onkeyup="FormManager.isInteger('+index+',this);" /></div><div id="sample'+index+'" class="sample">Exemplo:</div></div>';
	
},

addAttribute : function(){
	
	var ts = document.getElementById('layerAttributes0').textContent;
	
	if(ts != ""){
		this.attCount++;
	
		if(this.attCount == 1)
			document.getElementById('addedAttributeList').innerHTML +='<div id="dropButton" onclick="FormManager.dropAttribute();">Remover Atributo</div>';
	
		document.getElementById('addedAttributeList').innerHTML += this.createAttribute(this.attCount);
	}
},

dropAttribute : function(){
	
	if(this.attCount > 0){
	this.attCount--;
	
	var list = document.getElementById('addedAttributeList');
	
	list.removeChild(document.getElementById('addedAttributeList').lastChild);
	
	if(this.attCount == 0)
		list.removeChild(document.getElementById('addedAttributeList').firstChild);
	}
	
},

enableFilter : function() {
	
		var queryParams = '';

		for(i = 0; i != this.attCount+1; i++){
		
			var selectedOption = document.getElementById('layerAttributes'+i);
			
			if(selectedOption != null){
				
			var layerType = selectedOption.options[selectedOption.selectedIndex].id;
		
			if(i > 0){
				if(layerType.search("text") == -1 || layerType.search("character") == -1)
					queryParams += " AND "+selectedOption.value+" like '%"+ document.getElementById('filterValueField'+i).value + "%'";
				else
					queryParams += " AND "+selectedOption.value+" = "+ document.getElementById('filterValueField'+i).value;
			}
			else{
				if(layerType.search("text") == -1 || layerType.search("character") == -1)
					queryParams += selectedOption.value+" like '%"+ document.getElementById('filterValueField'+i).value + "%'";
				else
					queryParams += selectedOption.value+" = "+ document.getElementById('filterValueField'+i).value;
			}			
		   }
		}
		
	    ActionManager.sendRequest(queryParams,'query','records');
	    	    
},

isForeign: function(name){
	
	var fk ='';
	
	for(i =0; i != this.fs.length; i++)
		if(this.fs[i] == name)
			return true;
			
	return false;
	
},

checkUpdateForeign: function(fs,value){
	
	
	for(c = 0; c != this.fkData[fs].length; c++)
		if(this.fkData[fs][c][1] == value || this.fkData[fs][c][0] == value)
			return this.fkData[fs][c][0];
			
	return this.defaultComboValue;
	
},

checkInsertForeign: function(r){
	
	for(c = 0; c != this.comboHolder.length; c++)
		r.set(this.comboHolder[c][0],this.comboHolder[c][1]);
	
},

checkDefaultCombo:function(r){
	
	value = r.data[this.defaultCombo];
	
	if(value == ''){
		r.set(this.defaultCombo,this.defaultComboTextValue);
		this.data[this.defaultCombo] = this.defaultComboValue;
	}
	
}

};

function updateGridView(text){
	 
    var records = eval(text);
    		
    GridVariables.editor.stopEditing();
    GridVariables.store.removeAll();
    GridVariables.store.add(records);
    GridVariables.grid.getView().refresh();
	
}