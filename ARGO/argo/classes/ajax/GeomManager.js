GeomManager = {
coordCount: 0,
georg:'',
hasGeom:false,
type:'',
epsg:'',

generateGeoJson:function(){
	
	var coord = '';
	
	if(this.type == 'POINT' || this.type == 'Point')
		for(i = 0; i != this.coordCount+1; i++)
			coord +="["+document.getElementById('x'+i).value+","+document.getElementById('y'+i).value+"]";
	
	else if(this.type == 'MULTILINESTRING' || this.type == 'MultiLineString'){
		
		coord +="[[["+document.getElementById('x0').value+","+document.getElementById('y0').value+"]";
			
		for(i = 1; i != this.coordCount+1; i++)
			coord +=",["+document.getElementById('x'+i).value+","+document.getElementById('y'+i).value+"]";
	
		coord +="]]";
	}
	
	else if(this.type == 'MULTIPOLYGON' || this.type == 'MultiPolygon'){
		
		coord +="[[[["+document.getElementById('x0').value+","+document.getElementById('y0').value+"]";
			
		for(i = 1; i != this.coordCount+1; i++)
			coord +=",["+document.getElementById('x'+i).value+","+document.getElementById('y'+i).value+"]";
	
		coord +="]]]";
	}
	
	return '{"type":"'+this.type+'","coordinates":'+coord+'}';
	
},

translateGeoJson:function(geoJson){
            	
	eval('geoJson = '+geoJson);
            	
	return geoJson;
            	
},
            
writeGeoFields:function(geoJson){
            	
				GeomManager.clearGeomForm();
				
				if(geoJson && !geoJson['type'])
					geoJson = this.translateGeoJson(geoJson);
				
				if(geoJson){
            	
					var coordinates = geoJson['coordinates'];
            	
					if(geoJson['type'] == 'Point' || geoJson['type'] == 'POINT'){
            		document.getElementById('x0').value = coordinates[0];
            		document.getElementById('y0').value = coordinates[1];
					}
            	
					else if((geoJson['type'] == 'MultiLineString' || geoJson['type'] == 'MULTILINESTRING') & coordinates[0][0] != null){
            	
            		document.getElementById('x0').value = coordinates[0][0][0];
            		document.getElementById('y0').value = coordinates[0][0][1];
            
            		
            		for( k = 1; k != coordinates[0].length; k++){
            			
            			this.addValuedCoordinate(coordinates[0][k][0],coordinates[0][k][1]);
            		}
            		
            		}
            	
					else if((geoJson['type'] == 'MultiPolygon' || geoJson['type'] == 'MULTIPOLYGON') & coordinates[0][0] != null){
                	
            		document.getElementById('x0').value = coordinates[0][0][0][0];
            		document.getElementById('y0').value = coordinates[0][0][0][1];
            
            		
            		for( k = 1; k != coordinates[0][0].length; k++){
            			 this.addValuedCoordinate(coordinates[0][0][k][0],coordinates[0][0][k][1]);
            			}
            		
					}
            	}
},
  
createValuedCoordinate : function(index,x,y){
	      	
	return '<div id="coordinate'+index+'" class="coordinate"><div class="label">Long.(X): <input id="x'+index+'" class="coordinateValueField" onkeyup="GeomManager.isInteger(x'+index+',this);" type="text" value="'+x+'"/></div><div class="label">Latit.(Y): <input id="y'+index+'" class="coordinateValueField" onkeyup="GeomManager.isInteger(y'+index+',this);" type="text" value="'+y+'"/></div></div>';            	
	
},
            
addValuedCoordinate : function(x,y){
            	
		this.coordCount++;
            	
		if(this.coordCount == 1)
			document.getElementById('addedCoordinateList').innerHTML +='<div id="dropButton" onclick="GeomManager.dropCoordinate();">Remover Atributo</div>';
            	
		document.getElementById('addedCoordinateList').innerHTML += this.createValuedCoordinate(this.coordCount,x,y);
            
},

createCoordinate : function(index){
            	
	//var firstContent  = document.getElementById('layerCoordinates0').innerHTML;
            	
	return '<div id="coordinate'+index+'" class="coordinate"><div class="label">X: <input id="x'+index+'" class="coordinateValueField" onkeyup="GeomManager.isInteger(x'+index+',this);" type="text"/></div><div class="label">Y: <input id="y'+index+'" class="coordinateValueField" onkeyup="GeomManager.isInteger(y'+index+',this);" type="text"/></div></div>';            	
	
},
            
addCoordinate : function(){

		if(this.type != 'Point'){
			this.coordCount++;
            	
			if(this.coordCount == 1)
				document.getElementById('addedCoordinateList').innerHTML +='<div id="dropButton" onclick="GeomManager.dropCoordinate();">Remover Atributo</div>';
            	
			document.getElementById('addedCoordinateList').innerHTML += this.createCoordinate(this.coordCount);
		}
},
            
dropCoordinate : function(){
            	
	if(this.coordCount > 0){
		this.coordCount--;
            	
	var list = document.getElementById('addedCoordinateList');
            	
	list.removeChild(document.getElementById('addedCoordinateList').lastChild);
            	
	if(this.coordCount == 0)
		list.removeChild(document.getElementById('addedCoordinateList').firstChild);
    }
            	
},
	
clearGeomForm : function(){
	            	
		document.getElementById('addedCoordinateList').innerHTML = "";
	    
		document.getElementById('x0').value = "0";
		document.getElementById('y0').value = "0";
		
		this.coordCount = 0;
	
},
	
isInteger : function(id,campo){
				
		var digits="0123456789.-" ; 
		var campo_temp;   
		    for (var i=0; i < campo.value.length; i++){  
		        campo_temp=campo.value.substring(i,i+1);   
		        if (digits.indexOf(campo_temp)==-1){  
		            campo.value = campo.value.substring(0,i);  
		        }  
		    }  
}  
		
};