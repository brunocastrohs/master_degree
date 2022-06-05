<?php
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/DatabaseUtil.class.inc");

class GenericDAO {
	
	static $columns = null;
	
	static $georg = null;
	
	static $fks = null;
	
	static $defaultFk = '';
	
	static $defaultFkValue = '';

	static $defaultForeignValue = '';
	
	static $currentView = '';
	
	static $hasSubkume = false;
	
	static $kumeGid = '0';
	
	static $kumeDays = array();
	
	static $kumeDaysRestriction = array("1"=>"false","2"=>"false","3"=>"false","4"=>"false","5"=>"false");
	
	static function save($schema,$table,$object){
		
		if(isset(DatabaseConstants::$subKumeTables[$table])){
				
			unset($object['ordem_visita']);
			unset($object['bairro']);
			unset($object['subkume']);
			unset($object['kume']);
			unset($object['kumeDays']);
			unset($object['kumeDaysRestriction']);
				
		}

		if($table == 'usuario')
			$object['usu_senha'] = "'".md5($object['usu_senha'])."'";
		
		$sql = "INSERT INTO $schema.$table (". implode(",",array_keys($object)).") VALUES(".implode(",",array_values($object)).")";
		
		$rs = DataBaseUtil::execute($sql);

		return $rs;

	}

	static function update($schema,$table,$object,$pk,$id){
		
		if(isset(DatabaseConstants::$subKumeTables[$table])){
			
			if(isset($object['kume'])){
				$kumeHold = self::hasKumeChanged($schema, $table, $id);
						
				if($kumeHold!=$object['kume']){			
					$object['color'] = self::getNewKumeColor($schema, $table, $object['kume']);
					self::updateNewkume($id, $object['kume']);
				}
			}
			
			self::updateKumeDays($object, $id);
			
			unset($object['ordem_visita']);
			unset($object['bairro']);
			unset($object['subkume']);
			unset($object['kume']);
			unset($object['kumeDays']);
			unset($object['kumeDaysRestriction']);
			
		}
		

		$sql = "UPDATE $schema.$table SET ". self::generateUpadteString($object) ." WHERE $pk = $id";
		
		$rs = DataBaseUtil::execute($sql);

		return $rs;

	}

	static function remove($schema,$table,$pk,$id){

		$sql ="DELETE FROM $schema.$table WHERE $pk = $id";
		
		$rs = DataBaseUtil::execute($sql);

		return $rs;

	}

	//metadata
	static function retrieveSchemas(){

		$sql = "SELECT DISTINCT pg_tables.schemaname as schema FROM pg_tables WHERE pg_tables.schemaname = 'public' AND pg_tables.schemaname <> ALL (ARRAY['pg_catalog'::name, 'information_schema'::name, 'pg_toast'::name]) ORDER BY pg_tables.schemaname";

		$rs = DataBaseUtil::execute($sql);

		$list = array();
			
		while($row = pg_fetch_row($rs)){

			if(SecurityManager::checkPermission("VISUALIZAR_SCHEMA_".strtoupper($row[0])))
				$list[]['schema'] = $row[0];
			
		}

		return $list;

	}
		
	static function retrieveTables($schemas){

		$sql = "SELECT pg_tables.schemaname, pg_tables.tablename FROM pg_tables WHERE pg_tables.schemaname <> ALL (ARRAY['pg_catalog'::name, 'information_schema'::name, 'pg_toast'::name]) ".self::generateSchemasClause($schemas)." ORDER BY pg_tables.schemaname, pg_tables.tablename";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
			
		$list = array();
		
		$allowedTables = array('objeto'=>'','kume'=>'');
			
		while($row = pg_fetch_row($rs)){
				
			if( isset($allowedTables[$row[1]]) ){
				$list[$flag]['schema'] 	= $row[0];
				$list[$flag]['table'] 	= $row[1];
				$list[$flag]['label'] 	= self::retrieveTableLabel($row[0], $row[1]);
			
				$flag ++;
			}
		}

		return $list;

	}
	
	static function retrieveTableLabel($schema,$table){

	//	$sql = "select description from pg_description where objoid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and objsubid = 0";

	//	$rs = DataBaseUtil::execute($sql);

	//	while($row = pg_fetch_row($rs)){
	//		return $row[0];			
	//	}

		return ucwords(str_replace("_", " ", $table));

	}

	static function generateSchemasClause($schemas){

		$str = "";

		foreach($schemas as $schema){

			if($str == "")
				$str .= " AND pg_tables.schemaname = '{$schema['schema']}' ";
			else
				$str .= " OR pg_tables.schemaname = '{$schema['schema']}' ";
		}
		
		return $str;

	}
	
	static function retrieveIdCount($schema,$table, $pk){

		$sql = "select max($pk) from $schema.$table";

		$rs = DataBaseUtil::execute($sql);
			
		$list = array();
			
		while($row = pg_fetch_row($rs)){
			if($row[0] > 0)	
				return $row[0];
			else 
				return 0;
		}

	}
	
	static function retrievePrimaryKey($schema,$table){

		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod) FROM pg_index, pg_class, pg_attribute WHERE pg_class.oid = '$schema.$table'::regclass AND indrelid = pg_class.oid AND pg_attribute.attrelid = pg_class.oid AND pg_attribute.attnum = any(pg_index.indkey) AND indisprimary";

		$rs = DataBaseUtil::execute($sql);
			
		$list = array();
		
		$pk = '';
		
		while($row = pg_fetch_row($rs)){
				
			return $row[0];
			
		}
		
	}
	
	static function retrieveViewPrimaryKey($table){
		
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod) FROM pg_index, pg_class, pg_attribute WHERE pg_class.oid = '$table'::regclass AND indrelid = pg_class.oid AND pg_attribute.attrelid = pg_class.oid AND pg_attribute.attnum = any(pg_index.indkey) AND indisprimary";
	
		$rs = DataBaseUtil::execute($sql);
			
		$list = array();
		
		while($row = pg_fetch_row($rs)){
	
			return $row[0];
				
		}
	
	}
	
	static function retrieveForeignKeys($schema,$table){

		$sql = "SELECT DISTINCT kcu.column_name,ccu.table_schema,ccu.table_name,ccu.column_name FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND tc.table_name='$table' AND tc.table_schema = '$schema'";

		$rs = DataBaseUtil::execute($sql);

		$list = array();
			
		$flag = 0;
		
		if(!self::$hasSubkume  && isset(DatabaseConstants::$subKumeTables[$table])){
			array_push($list, array("fk"=>"kume","schema"=>"public","table"=>"kume_combobox","pk"=>"gid"));
		}
		
		while($row = pg_fetch_row($rs)){
				
			if($row[2]!= 'kume'){
				$list[$flag]['fk'] = $row[0];
				$list[$flag]['schema'] = $row[1];
				$list[$flag]['table'] = $row[2];
				$list[$flag]['pk'] = $row[3];
			}
			
			$flag++;
		}

		return $list;
		
	}
	
	static function retrieveForeignKey($schema,$table,$foreignSchema,$foreignTable){

		$sql = "SELECT DISTINCT kcu.column_name FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND tc.table_name='$table' AND tc.table_schema = '$schema' AND ccu.table_schema = '$foreignSchema' AND ccu.table_name = '$foreignTable'";

		$rs = DataBaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
				
			return $row[0];
		}

		return "";
		
	}

	static function retrieveForeignValues($schema,$table){

		//filtrar por subkume quando houver
		$fks = self::retrieveForeignKeys($schema, $table);
		
		$param = '';
		
		if(!$fks)
			return null;
		
		if(isset(DatabaseConstants::$subKumeTables[$table])){
			$param = " WHERE subkume = '.0.' or subkume is null";
		}
		
		$list = array();
		
		$flag = 0;
				
		foreach ($fks as $fk){
			
			$field = self::retrieveColumns($fk['schema'], $fk['table'],'');
					
			$sql = "SELECT ".$fk['pk'].",".$field[1]['column']." FROM ".$fk['schema'].".".$fk['table'] ." $param";

			$rs = DataBaseUtil::execute($sql);
		
			$flagFk = 0;
			
			while($row = pg_fetch_row($rs)){
				
				$list[$flag]['fk_records'][$flagFk]['id'] = $row[0];
				$list[$flag]['fk_records'][$flagFk]['value'] = $row[1];
				
				if(self::$defaultFk && self::$defaultFk == $fk['fk'] && self::$defaultFkValue == $row[0]){
					self::$defaultForeignValue = $row[1];
				}
				
				$flagFk++;
			}
			
			$list[$flag]['fk_name'] = $fk['fk'];
			
			$flag++;
			
		}
		
		return $list;
	}
	
	//retorna todas as colunas de uma tabela mais o tipo de cada respectiva coluna
	static function retrieveColumns($schema,$table,$clauses){

		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' and format_type(pg_attribute.atttypid, pg_attribute.atttypmod) <> 'geometry' $clauses";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
			
		$list = array();
		
	//	$prefix = '';
			
		//$isConstraintTable = self::isConstraintTable($schema, $table);
		
		while($row = pg_fetch_row($rs)){
				
			$list[$flag]['column'] = $row[0] ;
			$list[$flag]['type'] = $row[1];
			$list[$flag]['id'] = $row[2];
		//	$prefix = explode("_", $row[0]);
			$list[$flag]['label'] = isset(DatabaseConstants::$labels[$table])&&isset(DatabaseConstants::$labels[$table][$row[0]])?DatabaseConstants::$labels[$table][$row[0]]:ucwords(str_replace("_"," ",$row[0]));
			
			//if($list[$flag]['column'] == 'kume' && self::$hasSubkume)
			//	$list[$flag]['label'] = 'Dia';
			
			//echo $list[$flag]['label']."<br/>";
			//$list[$flag]['label'] = $row[0];
			
			if(self::$fks){
				$list[$flag]['type'] = self::checkForeignColumns($row[0],$row[1]);
				
				if($row[0] == self::$defaultFk){
					$list[$flag]['defaultValueText'] = self::$defaultForeignValue;
				}
			}
			
			//echo $list[$flag]['type'];
			
			if(strstr($list[$flag]['type'],'geometry') && !strstr($list[$flag]['type'],'Point')){
				unset($list[$flag]);
			}else if(strstr($list[$flag]['type'],'geometry') && strstr($list[$flag]['type'],'Point')){
				self::$georg = $list[$flag];
			}
				
			
			if( isset(DatabaseConstants::$hiddens[$table])&&isset(DatabaseConstants::$hiddens[$table][$row[0]])){
				$flag --;
			}
			
			if( self::$hasSubkume && $row[0] == 'kume'){
				$flag --;
			}
			
			$flag ++;
		}

		$list = self::hasForeigColumn($schema, $table,$list);
		
		return $list;

	}
	
	static function retrieveSelectGISColumns($schema,$table,$clauses){
	
		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' and format_type(pg_attribute.atttypid, pg_attribute.atttypmod) <> 'geometry' $clauses";
	
		$rs = DataBaseUtil::execute($sql);
	
		$flag = 0;
			
		$list = array();
	
		while($row = pg_fetch_row($rs)){
	
			$list[$flag]['column'] = $row[0] ;
			$list[$flag]['type'] = $row[1];
			$list[$flag]['id'] = $row[2];
			$list[$flag]['label'] = isset(DatabaseConstants::$labels[$table])&&isset(DatabaseConstants::$labels[$table][$row[0]])?DatabaseConstants::$labels[$table][$row[0]]:ucwords(str_replace("_"," ",$row[0]));
			
				
			if( isset(DatabaseConstants::$selectHiddens[$table])&&isset(DatabaseConstants::$selectHiddens[$table][$row[0]])){
				$flag --;
			}
					
			$flag ++;
		}
	
	
		return $list;
	
	}
	
	static function retrieveGISColumns($schema,$table,$clauses){

		$sql = "SELECT pg_attribute.attname,format_type(pg_attribute.atttypid, pg_attribute.atttypmod),attnum FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT oid FROM pg_class WHERE oid = '$schema.$table'::regclass) and pg_attribute.attnum > 0 and pg_attribute.ATTISDROPPED = 'f' $clauses";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
			
		$list = array();
		
		while($row = pg_fetch_row($rs)){
				
			$list[$flag]['column'] = $row[0] ;
			$list[$flag]['type'] = $row[1];
			$list[$flag]['id'] = $row[2];
		//	$prefix = explode("_", $row[0]);
			$list[$flag]['label'] = isset(DatabaseConstants::$labels[$table])&&isset(DatabaseConstants::$labels[$table][$row[0]])?DatabaseConstants::$labels[$table][$row[0]]:ucwords(str_replace("_"," ",$row[0]));
			
			$flag ++;
		}
		
		
		
		self::checkViewForm($schema, $table);
		
	//	$list = self::retrieveColumnLabels($schema,$table,$list);

		return $list;

	}
	
	static function checkForeignColumns($column,$type){
		
		foreach (self::$fks as $fk)
			if($fk['fk_name'] == $column ){
				return 'fk';
			}
		
		return $type;
		
	}
	//geom statics
	//excuta uma consulta a partir do nome da tabela e uma string de clausula where;
	static function retrieveRecords($schema, $table,$params){

		if($params != ''){
			$params = " WHERE $params";
		
			if(isset(DatabaseConstants::$subKumeTables[$table])){
				self::setSubKume($schema, $table, $params);
			}
			
		}
		
		self::$columns = $cols = self::retrieveColumns($schema,$table,'');
		
		$select = self::generateSelectColumns($cols);

		$sql = "SELECT $select FROM $schema.$table $params";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
			
		$records = array();
		
		//$isConstraintTable = self::isConstraintTable($schema, $table);
		
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){	
				
				$records[$flag][$c]['column'] = $cols[$c]['column'] ;
				$records[$flag][$c]['type'] = $cols[$c]['type'];
				$records[$flag][$c]['label'] = $cols[$c]['label'];
				if(isset($cols[$c]['isHidden']))
					$records[$flag][$c]['isHidden'] = $cols[$c]['isHidden'];
				
				if($cols[$c]['type'] == 'action')
					$records[$flag][$c]['value'] = $row[0];
				else
					$records[$flag][$c]['value'] = $row[$c];
			}
			
			$flag ++;
		}
	
		//gera um array fgk=>[valores]
		self::$fks = self::retrieveForeignValues($schema, $table);
		
		if(self::$fks){	
			//relaciona o valor da fg corrente ao array conteno o texto criado acima
			$records = self::generateForeignValues($records, self::$fks);
		}
		
		return $records;

	}

	static function generateForeignValues($records,$fks){
		
		$flag = 0;
		
		$c = 0;
		
		foreach ($fks as $fk){
			$flag = 0;
			foreach ($records as $row){
				$c = 0;
				foreach ($row as $column){
					if($column['column'] == $fk['fk_name']){
						$column['type'] = 'fk';
						$column['value'] = self::checkForeignValues($column['value'], $fk['fk_records']);
						$records[$flag][$c] = $column;
					}
				$c++;
				}
				$flag++;
			}
		}
		
		return $records;
		
	}
	
	static function checkForeignValues($id,$fk){
		
		foreach ($fk as $e)
			if($id == $e['id'])
				return $e['value'];
		
	}
	
	//geom statics
	static function retrieveReportRecords($table, $cols, $params){

		if($params != '')
			$params = " WHERE $params";
			
		$keys = array_keys($cols);
			
		$select = implode(',', $keys);

		$sql = "SELECT $select FROM $table $params ";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
		
		$records = array();
		
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($keys); $c++){	
			
					$records[$flag][$c]['column'] = $cols[$keys[$c]] ;
					$records[$flag][$c]['value'] = $row[$c];
				
			}
			$flag ++;
		}
	
		return $records;

	}
	
	static function retrieveReportData($schema,$table, $cols, $params){
	
		if($params != '')
			$params = " WHERE $params";
			
		$keys = explode(',',$cols);
			
		$select = $cols;
	
		$sql = "SELECT $select FROM $schema.$table $params";
	
	//	echo mb_convert_encoding($sql,'UTF-8', 'ISO-8859-1');
		
		$rs = DataBaseUtil::execute($sql);
	
		$flag = 0;
	
		$records = array();
	
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($keys); $c++){
					
				$records[$flag][$c]['column'] = $keys[$c] ;
				$records[$flag][$c]['value'] = $row[$c];
	
			}
			$flag ++;
		}
	
		return $records;
	
	}
	
	static function retrieveTitles($table, $cols){
	
		$list = array();
		
		//$prefix = '';
		
		$cols = explode(',',$cols);
	
		foreach($cols as $col){
		//	$prefix = explode("_", $col);
			$list[$col] = isset(DatabaseConstants::$labels[$table])&&isset(DatabaseConstants::$labels[$table][$col])?DatabaseConstants::$labels[$table][$col]:ucwords(str_replace("_"," ", $col));
		}
		
		return $list;
	
	}
	
	//gis
	//gid statics
	static function retrieveLayers($containers){

		$flag = 0;
			
		$records = array();
		
		foreach ($containers as $container){
		
		$cols = array('gid','name','title','url','visibility','group','schema','container');

		$sql = 'SELECT lay_codigo, lay_nome,lay_titulo,lay_url,lay_visibilidade,lay_estilo,lay_schema,con_nome FROM gis.layer,gis.container WHERE con_codigo = lay_container AND con_codigo = '.$container[0]['value'].' ORDER BY lay_codigo';

		$rs = DataBaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){	
				$records[$flag][$c]['value'] = $row[$c] ;
				$records[$flag][$c]['column'] = $cols[$c];
			}
			
			$flag ++;
		}
		}
		
		return $records;

	}
	
	static function retrieveBairros(){

		    $str = "[";
	
			$sql = 'SELECT gid,nome FROM bairro ORDER BY nome';
	
			$rs = DataBaseUtil::execute($sql);

			$isFirst = true;
			
			while($row = pg_fetch_row($rs)){
				
				if($isFirst)
					$str .= "['{$row[0]}','{$row[1]}']";
				else
					$str .= ",['{$row[0]}','{$row[1]}']";

				$isFirst = false;
				
			}
		
			$str .= "]";
			
		return $str;
	
	}
	
	//gid statcis
	static function retrieveContainers($id){

		$cols = array('gid','name','radio');

		$sql = "SELECT DISTINCT con_codigo, con_nome,con_radio FROM gis.layer,gis.container,gis.projeto WHERE con_codigo = lay_container AND pro_codigo = con_projeto AND pro_codigo = $id  ORDER BY con_codigo";

		$rs = DataBaseUtil::execute($sql);

		$flag = 0;
			
		$records = array();
		
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){	
				$records[$flag][$c]['value'] = $row[$c] ;
				$records[$flag][$c]['column'] = $cols[$c];
			}
			
			$flag ++;
		}
		
		return $records;

	}
	//statics public static geom
	static function retrieveGISRecords($schema,$table,$params){

		$cols 	= self::retrieveGISColumns($schema,$table,'');
		
		$select = self::generateSelectColumns($cols);

		$geom 	= self::retrieveGeomColumn($schema, $table);
		
		//echo "$select ---- $geom";
		
		$select = str_replace("st_asgeojson($geom)", "st_astext(st_centroid($geom))", $select);

		$sql = "SELECT $select FROM $schema.$table  WHERE $params ";

		$rs = DatabaseUtil::execute($sql);

		$flag = 0;
			
		$list = array();
		
		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){	
				$list[$flag][$c]['column'] = $cols[$c]['column'];
				$list[$flag][$c]['label'] = $cols[$c]['label'];
				isset(DatabaseConstants::$labels[$table])&&isset(DatabaseConstants::$labels[$table][$cols[$c]['column']])?DatabaseConstants::$labels[$table][$cols[$c]['column']]:ucwords(str_replace("_"," ",$cols[$c]['column']));
				$list[$flag][$c]['type'] = $cols[$c]['type'];
				
				if(isset($cols[$c]['isHidden']))
					$list[$flag][$c]['isHidden'] = $cols[$c]['isHidden'];
				
				if($list[$flag][$c]['column'] != $geom)
					$list[$flag][$c]['value'] = $row[$c];
				else
					$list[$flag][$c]['value'] = str_replace(')', '',str_replace('POINT(', '', $row[$c]));
			}
			$flag ++;
		}

		return $list;

	}
	
	static function retrieveAutoCompleteRecords($schema,$table,$column,$index,$params){
		
		$sql = "SELECT DISTINCT $column FROM $schema.$table  WHERE $params ORDER BY $column limit 5";
		
		$rs = DatabaseUtil::execute($sql);
			
		echo "<div class='autoList'>";
	
		while($row = pg_fetch_row($rs)){
			
			echo "<div onclick='MapAction.getAutoValue({$index},".'"'.$row[0].'"'.")' class='autoItem'><a href='#'>{$row[0]}</a></div>";
			
		}
	
		echo "</div>";
	
	}
	
	static function retrieveGeomColumn($schema,$table){

		$sql = "SELECT f_geometry_column FROM geometry_columns WHERE f_table_schema = '$schema' AND f_table_name = '$table'";

		$rs = DatabaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
			return $row[0];
		}

	}

	static function getGeomType(){

		$type = explode(',', self::$georg['type']);
		
		$type = explode('(',$type[0]); 
		
		return $type[1];
		
	}
	
	static function getGISSRID(){
					
		$type = explode(',', self::$georg['type']);
		
		$type = str_replace(')','',$type[1]); 
		
		return $type;
		
	}
		
	static function retrieveGeomType($schema,$table){

		$sql = "SELECT type FROM geometry_columns WHERE f_table_schema = '$schema' AND f_table_name = '$table'";

		$rs = DatabaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
			return $row[0];
		}
		
	}
	
	static function retrieveGISSRID($schema,$table){

		$sql = "SELECT srid FROM geometry_columns WHERE f_table_schema = '$schema' AND f_table_name = '$table'";

		$rs = DatabaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
			return $row[0];
		}
		
		return '4326';

	}

	//gerar a string com nome de tabelas para clausula select
	static function generateSelectColumns($columns){
			
		$string = "";
			
		$flag = 0;
			
		foreach($columns as $element){

			if(!isset($element['foreignTable'])){
				if($flag == 0)
					$string .= $element['column'];
				else if(strstr($element['type'], 'geometry'))
					$string .= ",st_asgeojson({$element['column']})";
				else 
					$string .= ",{$element['column']}";

			$flag++;
			}
			
		}
			
		if($string == "")
		return "*";
			
		return $string;
			
	}
	
	static function generateUpadteString($list){
		
		$updateStr = '';
		
		$keys = array_keys($list);
		
		$values = array_values($list);
		
		$isFirst = true;
		
		for($i = 0; $i != count($keys); $i++){
			if($isFirst){
				$updateStr .= $keys[$i]." = ".$values[$i];
				$isFirst = false;
			}else 
				$updateStr .= " ,".$keys[$i]." = ".$values[$i];
			
		}
		
		return $updateStr;
		
	}

	//user session
	//gid statcis
	static function retrieveUser($login, $pass){

		$user = array();

		$sql = "SELECT usu_codigo,usu_nome FROM seguranca.usuario where usu_login = '$login' AND usu_senha = md5('$pass')";

		$rs = DataBaseUtil::execute($sql);

		while($row = pg_fetch_row($rs)){
			
			$user['id'] = $row[0];
			$user['nome'] = $row[1];
			
		}
		
		if(!isset($user['id']))
			return false;
		
			
		$user['permissions'] = self::retrieveUserPermissions(self::retrieveUserGroups($user['id']));
		
		if(!$user['permissions'])
			return false;
		
		
		return $user;

	}
		//gid statcis
	static function retrieveUserGroups($id){

		$sql = "SELECT ugr_grupo FROM seguranca.usuario,seguranca.grupo,seguranca.usuario_grupo where grupo.gru_codigo = ugr_grupo AND usuario.usu_codigo = ugr_usuario AND usuario.usu_codigo = $id";

		$rs = DataBaseUtil::execute($sql);

		$list = array();
		
		while($row = pg_fetch_row($rs)){
			
			$list[] = $row[0];
			
		}

		return $list;
		
	}
	
	static function retrieveUserPermissions($list){

		if(count($list)){
			$sql = "SELECT per_nome FROM seguranca.permissao,seguranca.grupo,seguranca.grupo_permissao where gru_codigo = gpe_grupo AND per_codigo = gpe_permissao";

			foreach ($list as $id)
				$sql .=  " AND gru_codigo = $id";
		
			$rs = DataBaseUtil::execute($sql);

			$list = array();
		
			while($row = pg_fetch_row($rs)){
			
				$list[] = $row[0];
			
			}

			return $list;
		}	

		return false;
		
	}

	static function hasForeigColumn($schema,$table,$list){
		
		$flag = count($list);
		
		if(array_key_exists("$schema.$table", DatabaseConstants::$foreignTables)){
			foreach (DatabaseConstants::$foreignTables["$schema.$table"] as $foreignTable){

				$foreignData = explode('.',$foreignTable);
				
				$column = explode('_',$foreignData[1]);
				
				$list[$flag]['column'] = isset($column[1])?$column[1]:$column[0];
				$list[$flag]['label'] = self::retrieveTableLabel($foreignData[0], $foreignData[1]);
				$list[$flag]['type'] = 'action';
				$list[$flag]['foreignSchema'] = base64_encode($foreignData[0]);
				$list[$flag]['foreignTable'] = base64_encode($foreignData[1]);
				$list[$flag]['foreignColumn'] = base64_encode(self::retrieveForeignKey( $foreignData[0], $foreignData[1],$schema, $table));
				
				$flag++;
			}
			
		}
		
		return $list;
		
	}

	static function checkViewForm($schema,$table){

		if(isset(DatabaseConstants::$tableViews["$schema.$table"])){
			self::$currentView = DatabaseConstants::$tableViews["$schema.$table"];
			return self::$currentView;
		}
		
		return $table;
	}
	
	static function checkNumber($schema,$table){
	
		$user = array();
	
		$sql = "SELECT count(*) FROM $schema.$table";
	
		$rs = DataBaseUtil::execute($sql);
	
		while($row = pg_fetch_row($rs)){

			echo "Número de Registros: $row[0].";
			
		}
		
		//echo 'Número de Registros: 0.';

		
	}
	
	static function setSubKume($schema,$table,$params){
	
		$sql = "SELECT subkume,kume FROM $schema.$table $params";

		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
		
			self::$hasSubkume = "$row[0]";
			
			self::$kumeGid = "$row[1]";
			
			$gid = explode('=', $params);
			
			self::checkObjectKumeDay( $row[0]);
			
			self::checkObjectKumeDayRestriction($gid[1]);
			
			
		}
	
	}
	
	static function checkObjectKumeDay($gid){
	
		$sql = "SELECT gid,descricao FROM kume WHERE subkume = $gid";
	
		$rs = DataBaseUtil::execute($sql);
		
		while($row = pg_fetch_row($rs)){
	
			$checked = strstr(self::$kumeGid, ".{$row[0]}.")?'true':'false';
			
			array_push(self::$kumeDays,array("gid"=>$row[0],"descricao"=>$row[1],"checked"=>$checked));
			
		}
	
	}
	
	static function checkObjectKumeDayRestriction($gid){
	
		$sql = "SELECT dia FROM restricao WHERE objeto = $gid";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			if(isset(self::$kumeDaysRestriction [''.$row[0]]))
				self::$kumeDaysRestriction[''.$row[0]] = 'true';
	
		}
	
	}
	
	static function hasKumeChanged($schema,$table,$gid){
	
		$sql = "SELECT kume FROM $schema.$table ,objeto_kume WHERE objeto = gid and gid = $gid";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			return "$row[0]";
				
		}
	
	}
	
	static function getNewKumeColor($schema,$table,$gid){
		
		$gid = str_replace("'",'',str_replace('.', '', $gid));
	
		$sql = "SELECT color FROM objeto,objeto_kume WHERE objeto = gid AND (kume = $gid OR subkume = $gid) limit 1";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			return "$row[0]";
	
		}
	
	}
		
	static function updateNewkume($gid,$kume){
			
		DataBaseUtil::execute("UPDATE OBJETO_KUME SET KUME = ".str_replace('.', '', $kume) . "WHERE objeto = $gid");
			
		
	}
	
	static function updateKumeDays($object,$id){
				
			if(isset($object['subkume']) && isset($object['kumeDays'])){
				
				DataBaseUtil::execute("DELETE FROM OBJETO_KUME WHERE objeto = $id");
				
				$days  = explode("atend", $object['kumeDays']);
				
				foreach ($days as $day){
					
					$day = str_replace("'", "", $day);
					
					if(!strstr($day, 'undefined'))
						DataBaseUtil::execute("INSERT INTO OBJETO_KUME(OBJETO,KUME,SUBKUME) VALUES($id,$day,{$object['subkume']})");
					
				}				
			}
			
			if(isset($object['kumeDaysRestriction'])){
			
				DataBaseUtil::execute("DELETE FROM restricao WHERE objeto = $id");
			
				$days  = explode("restricao", $object['kumeDaysRestriction']);
				
				foreach ($days as $day){
					
					$day = str_replace("'", "", $day);
					
					if(!strstr($day, 'undefined'))
						DataBaseUtil::execute("INSERT INTO restricao(OBJETO,dia) VALUES($id,$day)");
						
				}			
				
			}
			
	
	}
	

	//new via INSERT
	//INSERT INTO NEW_VIA VALUES(100089,'','R','TF',(SELECT ST_MLineFromText('MULTILINESTRING((' || (SELECT ST_X(GEOM) ||  ' ' || ST_Y(GEOM) FROM NEW_NODE WHERE GID = 19893 ) || ', ' || (SELECT ST_X(GEOM) ||  ' ' || ST_Y(GEOM) FROM NEW_NODE WHERE GID = 19810 ) || '))',900913)))
	
	static function nearestNode($lat,$long){
	
		
		$sql = "select gid from new_node order by st_distance(ST_SETSRID(ST_MakePoint($long,$lat),900913),geom) asc limit 1";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			return "$row[0]";
	
		}
	
	}
	
	static function nearestEdge($lat,$long){
	
	
		$sql = "select gid,getedgename(gid) from new_edge order by st_distance(ST_SETSRID(ST_MakePoint($long,$lat),900913),geom) asc limit 1";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			return "$row[0]&&$row[1]";
	
		}
	
	}
	
	static function nearestVia($lat,$long){
	
		$sql = "select type || ' '|| name,l_f_add || '-' || l_t_add,r_f_add||'-'||r_t_add from via order by st_distance(geom,st_geomfromewkt('SRID=900913;POINT($long $lat)')) limit 1";
	
		$rs = DataBaseUtil::execute($sql);
			
		while($row = pg_fetch_row($rs)){
	
			echo "<b>Endereço: {$row[0]}; </b><br/>Números Lado Esquerdo:{$row[1]}; <br/>Números Lado Direito:{$row[2]}.";
	
		}
	
	}
	
	static function mergeEdge($geom1,$geom2,$one_way,$node){
		
	
		DataBaseUtil::execute("UPDATE NEW_NODE SET NEXT = NEXT ||'$geom2'||';' WHERE GID = $geom1");
		
		if($one_way == 'DB'){
			DataBaseUtil::execute("UPDATE NEW_NODE SET NEXT = NEXT ||'$geom1'||';' WHERE GID = $geom2");
		}
		
		if($one_way == 'TF'){
			$geom = "(ST_MAKELINE( (SELECT GEOM FROM NEW_NODE WHERE GID = $geom2),(SELECT GEOM FROM NEW_NODE WHERE GID = $geom1) ))";
		}
		else
			$geom = "(ST_MAKELINE( (SELECT GEOM FROM NEW_NODE WHERE GID = $geom1),(SELECT GEOM FROM NEW_NODE WHERE GID = $geom2) ))";
		
		DataBaseUtil::execute("INSERT INTO NEW_EDGE(GID,NODES,ONE_WAY,GEOM) VALUES((SELECT MAX(GID) +1 FROM NEW_EDGE),'$node','$one_way',$geom)");
	
	
	}
	
	static function mergeRout($geom1){
	
		DataBaseUtil::execute("INSERT INTO dijkstran_sequence(GID,edge_gid,indice) VALUES((SELECT MAX(GID) +1 FROM dijkstran_sequence),$geom1,(SELECT MAX(indice) +1 FROM dijkstran_sequence))");
	
		$rs = DataBaseUtil::execute("select sum(st_length(geom)/1000) from dijkstran_way");
			
		while($row = pg_fetch_row($rs)){
		
			return $row[0];
		
		}
	
	}
	
	static function dropRout($GID){
	
		DataBaseUtil::execute("DELETE FROM dijkstran_sequence WHERE EDGE_GID = $GID");
	
		$rs = DataBaseUtil::execute("select sum(st_length(geom)/1000) from dijkstran_way");
			
		while($row = pg_fetch_row($rs)){
		
			return $row[0];
		
		}
		
	}
	
}
?>