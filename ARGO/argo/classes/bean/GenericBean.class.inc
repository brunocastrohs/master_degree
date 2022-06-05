<?php
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/SecurityManager.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/libs/PHPLiveX/PHPLiveX.php");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/EXT.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/ReportUtil.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/GISManager.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/dao/GenericDAO.class.inc");

class GenericBean {

	static $ajax;
	
	static function save($shema,$table,$object){
	
		$rst = GenericDAO::save($shema,$table, $object);

		if ($rst) {
			HTML::generateJsScript("MessageManager.showMessage('".HTML::INSERTED."','success');");
		} else {
			HTML::generateJsScript("MessageManager.showMessage('".HTML::NOT_INSERTED."','error');");
		}


	}
	
	static function update($shema,$table,$object,$pk,$id){

		$rs = GenericDAO::update($shema,$table,$object,$pk,$id);

		if ($rs) {

			HTML::generateJsScript("MessageManager.showMessage('".HTML::UPDATED."','success');");

		} else {

			HTML::generateJsScript("MessageManager.showMessage('".HTML::NOT_UPDATED."','error');");

		}


	}

	static function remove($shema,$table,$pk,$id){

		$rst = GenericDAO::remove($shema,$table,$pk,$id);

		if ($rst) {

			HTML::generateJsScript("MessageManager.showMessage('".HTML::REMOVED."','success');");

		} else {

			HTML::generateJsScript("MessageManager.showMessage('".HTML::NOT_REMOVED."','error');");

		}


	}

	static function generateLoginBar(){

		EXT::generateLoginBar();
		
	}
	
	static function generateLoginPage(){

		include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
		
		HTML::generateJsScript(EXT::generateLoginPage());
		
		include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
		
	}
	
	static function generateToolBar(){

		$schemas = GenericDAO::retrieveSchemas();
		
		$tables = GenericDAO::retrieveTables($schemas);

		if(SecurityManager::checkSession() && count($schemas)){
			
			EXT::$menuReports = ReportConstants::retrieveReports();
			
			EXT::generateScrolls($schemas);
		
			EXT::generateScrollMenuItens($tables);
		
			EXT::generateMenu($schemas);
			
			HTML::generateJsScript(EXT::generateToolBar());
			
		}
		else 
			HTML::generateJsScript(EXT::generateLoginBar());
			
		
		
	}
	
	static function generateForm($schema, $table, $id,$lat,$long){

		$params = '';
		
		if(isset(DatabaseConstants::$tableViews["$schema.$table"])){
			EXT::$view 	= $table;
			$params = explode('.',DatabaseConstants::$tableViews["$schema.$table"]);
			EXT::$schema = $params[0];
			EXT::$table = $params[1];
			EXT::$pk 	= GenericDAO::retrievePrimaryKey(EXT::$schema,EXT::$table);
		//	
		}
		else{
			EXT::$schema 	= $schema;
			EXT::$table 	= $table;
			EXT::$pk	    = GenericDAO::retrievePrimaryKey($schema,$table);
		}
		
		//EXT::$table = GenericDAO::checkViewForm($schema,$table);
		
		//EXT::$pk = GenericDAO::retrievePrimaryKey($schema,EXT::$table);
	
		if($id != ''){
			
			$params = EXT::$pk." = $id";
		
			$records = GenericDAO::retrieveRecords($schema,$table,$params);
						
			EXT::$fks = GenericDAO::$fks;
			
			EXT::generateUpdateFormFields($records);
			
		}
		else{
				
			EXT::$idCount = 1+GenericDAO::retrieveIdCount($schema, EXT::$table, EXT::$pk);
			
			$columns = GenericDAO::retrieveColumns($schema, $table,'');
			
			//var_dump(GenericDAO::$fks);
			
			//echo "Here";
			
			EXT::$fks = GenericDAO::$fks;
			
			EXT::generateInsertFormFields($columns,$lat,$long);
		}
		
		if(EXT::$hasGeom == 'true'){
			EXT::$epsg = GenericDAO::getGISSRID();
		}
		
		//EXT::$fks = GenericDAO::$fks;
		
		EXT::generateFormToolBar($id);
		
		include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
		
		HTML::generateJsScript(EXT::generateForm());
		
		include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");

	}
	
	static function generateColumnScreen($schema, $table, $param){
		
		if($table == 'posto_macro_regiao' || $table == 'posto_municipio' || $table == 'posto_900913' || $table == 'posto_regiao_hidrografica')
			$table = 'posto_consulta';
		
		$param = str_replace("like", "ilike", $param);
	
		EXT::$schema = $schema;
		
		EXT::$table = $table;
		
		EXT::$param = $param;
		
		$html = self::generateColumnBoxes($schema, $table);
		
		HTML::generateJsScript(EXT::generateColumnScreen($html));
	
	}
	
	static function generateColumnBoxes($schema, $table){
	
		$columns = GenericDAO::retrieveColumns($schema, $table, '');
	
		$checked = " checked='checked' ";
	
		$flag = 0;
		
		$str = "<div id='boxes'>";
		
		foreach($columns as $column){
				
			if($flag > 5)
				$checked = '';
				
			$str .= "<div id='' style='width:250px; padding:3px 3px 3px 3px; float:left;'><input id='{$column['column']}' $checked value='{$column['column']}' type='checkbox' /> {$column['label']} </div>";
				
			$flag++;
				
		}
		
		$str .= "</div>";
		
		return $str;
	
	}
	
	static function generatePDFReport($schema, $table, $cols, $params){
		
		$headers = GenericDAO::retrieveTitles($table, $cols);
		
		$data = ReportUtil::parseRecordToData(GenericDAO::retrieveReportData($schema, $table, $cols, $params));
		
		ReportUtil::generatePDF('FUNCEME - RELATÓRIO', '', '8', $headers, $data);
	
	
	}
	
	static function generateXLSReport($schema, $table, $cols, $params){
		
		
	
		$headers = GenericDAO::retrieveTitles($table, $cols);
		
		$data = ReportUtil::parseRecordToData(GenericDAO::retrieveReportData($schema, $table, $cols, $params));
		
		$cols = explode(',',$cols);
		
		ReportUtil::generateXLS($cols,$headers,$data);
		
		//$records = GenericDAO::retrieveRecords($schema,$table,$param);
	
		//ReportUtil::generateReport($table, '', '8', ReportUtil::parseRecordToData($records));
	
		//EXT::generateUpdateFormFields($records);
	
	
	}
	
	static function generateTXTReport($schema, $table, $cols, $params){
	
		$headers = GenericDAO::retrieveTitles($table, $cols);
		
		$data = ReportUtil::parseRecordToData(GenericDAO::retrieveReportData($schema, $table, $cols, $params));
		
		$cols = explode(',',$cols);
		
		ReportUtil::generateTXT($cols,$headers,$data);
	
	
	}
	
	static function generateRowGrid($schema, $table){

		//EXT::$hasGeom == 'false';
		
		EXT::$pk = GenericDAO::retrievePrimaryKey($schema,$table);

		$records = GenericDAO::retrieveRecords($schema,$table,'');

	//	$columns = GenericDAO::$columns;
		
		$columns = GenericDAO::retrieveColumns($schema, $table,'');

		EXT::$fks = GenericDAO::$fks;

		EXT::$schema = $schema;
		
		EXT::$table = $table;
		
		EXT::$idCount = GenericDAO::retrieveIdCount($schema, $table, EXT::$pk);
		
		EXT::generateEntitiesData($records,false);

		if(EXT::$hasGeom == 'true'){
			EXT::$geomType = GenericDAO::getGeomType();
			EXT::$epsg = GenericDAO::getGISSRID();
		}
		
		EXT::generateDefaultEntities($columns);
				
		EXT::generateEntityColumns($columns);
		
		EXT::generateGridColumns($columns);
		
		EXT::generateGridToolBar();
		
		HTML::generateJsScript(EXT::generateRowGrid());

	}
	
	static function generateForeignRowGrid($schema, $table,$fk, $id){

		EXT::$hasGeom == 'false';
		
		EXT::$pk = GenericDAO::retrievePrimaryKey($schema,$table);

		GenericDAO::$defaultFk = $fk;
		
		GenericDAO::$defaultFkValue = $id;
		
		$records = GenericDAO::retrieveRecords($schema,$table," $fk = $id");

		//$columns = GenericDAO::$columns;
		
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
			EXT::$geomType = GenericDAO::getGeomType();
			EXT::$epsg = GenericDAO::getGISSRID();
		}
		
		EXT::generateDefaultEntities($columns);
				
		EXT::generateEntityColumns($columns);
		
		EXT::generateGridColumns($columns);
		
		EXT::generateGridToolBar();
		
		HTML::generateJsScript(EXT::generateRowGrid());

	}
	
	static function retrieveRecords($schema, $table, $params,$pk){

		$records = GenericDAO::retrieveRecords($schema, $table,$params,$pk);

		EXT::generateEntitiesData($records,true);
		
		HTML::generateJsScript("updateGridView(".EXT::$entityData.");");
		
	}
	
	static function retrieveGISRecords($schema,$table, $params){
	
		$params = str_replace("like", "ilike", $params);
	
		$rows = GenericDAO::retrieveGISRecords($schema,$table,$params);
		
		$isGeocod = $table == 'cliente_geocod'?true:false;
			
		if(isset($rows[0])){
			
			$tableSize = count($rows[0])+1;
	
			if($isGeocod){
				$tableSize++;
			}
			
			$size = 100/$tableSize;
	
			$flag = 0;
	
			$htmlTable ="<div id='size' style='overflow:scroll; width:inherit; height:360px;'><TABLE class='gisRecords' cellspacing='0'   ><TR><THEAD>";
	
			foreach($rows[0] as $col){
				$htmlTable .= HTML::createColumn($col['label'],$size,true);
			}
			
			if($isGeocod){
				$htmlTable .= HTML::createColumn('G. REVES',$size,true);
			}
	
			$htmlTable .="</THEAD></TR><TBODY>";
	
			foreach($rows as $row){
					
				$htmlTable .= '<TR>';
					
				foreach ($row as $col){
					if(!strstr($col['type'],'geometry'))
						$htmlTable .= $isGeocod && $col['column'] == 'address'?HTML::createGeoCodColumn($col['value'],$size,$flag):HTML::createColumn($col['value'],$size,false);
					else{
						$htmlTable .= HTML::createGeoColumn($col['value'],$size);
						if($isGeocod){
							$htmlTable .= HTML::createReverseGeoCodColumn($col['value'],$size);
						}
					}
				}
					
				$htmlTable .= '</TR>';
					
				$flag++;
			}
	
			$htmlTable .= "</TBODY></TABLE></div><div id='number' style='visibility:hidden;'>$flag</div>";
	
			echo $htmlTable;
		}
		else
			echo "<div id='size' style='display:none;'>3</div>Nenhum resultado encontrado para a sua consulta.";
	
	}

	static function retrieveAutoCompleteRecords($schema,$table,$column,$index, $params){
		
		$params = str_replace("like", "ilike", $params);
		
		GenericDAO::retrieveAutoCompleteRecords($schema,$table,$column,$index,$params);
		
	
	}

	static function retrieveColumnsOptions($schema,$table,$clauses){

		if($schema == '')
			$schema = 'public';
		
		$list = GenericDAO::retrieveColumns($schema,$table,$clauses);

		$rows = HTML::createOption('', '');
		 
		foreach($list as $entity){

			$rows .= HTML::createEnhancedOption("id='".$entity['type']."'",$entity['column'], $entity['label']);

		}
		 
		 
		return $rows;


	}

	static function retrieveGISColumns($json){
		
		echo "Hi there";

		if($json != ''){

		$json = (array)($json);
		
		$clauses = "and (format_type(pg_attribute.atttypid, pg_attribute.atttypmod) like '%character%' or format_type(pg_attribute.atttypid, pg_attribute.atttypmod) like '%text%')";
		
		GenericDAO::checkViewForm($json['x'], $json['y']);
		
		$list = GenericDAO::retrieveSelectGISColumns($json['x'],$json['y'],$clauses);

		$rows = HTML::createOption('', '');
		 
		foreach($list as $entity){

			$rows .= HTML::createEnhancedOption("id='".$entity['type']."'",$entity['column'], $entity['label']);

		}
		 
		return $rows;
		
		}

	}
		
	static function ajaxify(){
		
		$ajax = new PHPLiveX();

    	$ajax->AjaxifyClassMethods(array(get_class($this) => get_class_methods($this)));
    	
    	$ajax->Run();
    	
	}
	
	static function ajaxifyMethods($methods){
		
		$ajax = new PHPLiveX();

		//$class = get_class($this);
    	
		$ajax->AjaxifyClassMethods(array('GenericBean' => $methods));

 		$ajax->Run();
		
	}
}
?>
