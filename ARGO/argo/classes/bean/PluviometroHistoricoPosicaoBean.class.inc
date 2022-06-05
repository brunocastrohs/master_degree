<?php
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");

class PluviometroHistoricoPosicaoBean extends GenericBean{

	static function generateForeignRowGrid($schema, $table,$fk, $id){

		EXT::$hasGeom == 'false';
		
		EXT::$pk = GenericDAO::retrievePrimaryKey($schema,$table);

		GenericDAO::$defaultFk = $fk;
		
		GenericDAO::$defaultFkValue = $id;
		
		$records = GenericDAO::retrieveRecords($schema,$table," $fk = $id");

		$columns = GenericDAO::retrieveColumns($schema, $table,'');

		EXT::$fks = GenericDAO::$fks;
		
		EXT::$schema = $schema;
		
		EXT::$table = $table;
		
		EXT::$defaultCombo = $fk;
		
		EXT::$defaultComboValue = $id;
		
		EXT::$defaultComboTextValue = GenericDAO::$defaultForeignValue;
		
		EXT::$idCount = GenericDAO::retrieveIdCount($schema, $table, EXT::$pk);
		
		EXT::generateEntitiesData($records,false);

		if(EXT::$hasGeom == 'true'){
			EXT::$geomType = GenericDAO::retrieveGeomType($schema, $table);
			EXT::$epsg = GenericDAO::retrieveGISSRID($schema, $table);
		}
		
		EXT::generateDefaultEntities($columns);
				
		EXT::generateEntityColumns($columns);
		
		EXT::generateGridColumns($columns);
		
		EXT::generateGridToolBar();
		
		$params = "$fk = $id";
		
		EXT::$gridToolBar .= ",{iconCls: 'scrollReportItem',text: 'Relatório',handler: function(){
        	window.open('/ARGO/classes/receiver/GenericReceiver.php?action=report&x=".base64_encode('historico_posicao')."&y=".base64_encode($params)."');
    	}}";
				
		HTML::generateJsScript(EXT::generateRowGrid());

	}
	
}
?>
