<?php 
include_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/bean/GenericBean.class.inc");

class TMPUtil{
	
function transform($data){
	
	$cols = array('gid','municipio','posto','codigo','intervalo','ativo','contato','data_manutencao','geom');
	
	$flag = 0;
	
	$list = array();
	
	foreach ($data as $row){
		foreach (explode(';', $row) as $value) {
			
			if($cols[$flag] == 'ativo')
				$list[$cols[$flag]] = 'null';
			else if($cols[$flag] == 'data_manutencao')
				$list[$cols[$flag]] = 'null';
			else if($cols[$flag] == 'geom'){
				
			    $list[$cols[$flag]] = str_replace('"{"type"', "st_setsrid(st_geomfromgeojson('".'{"type"', $value);
			
				$list[$cols[$flag]] = str_replace("]}", "]}'),900913)", $list[$cols[$flag]]);
				
				$list[$cols[$flag]] = str_replace(')"',")", $list[$cols[$flag]]);
				
			}
			else if(strstr($value,'"' ))
				$list[$cols[$flag]] = str_replace('"', "'", $value);
			else
				$list[$cols[$flag]] = $value;
			
			$flag++; 
		}
		$flag = 0;
		GenericDAO::save('public', 'pluviometro', $list);
		echo "inserted:";
		print_r($list);
		echo "<br/>";
	}
	
}

static function generatePermissions(){
	
	$tables = GenericDAO::retrieveTables(array());
	
	$permissions = array('EDITAR','VISUALIZAR','REMOVER','INSERIR');
	
	foreach ($tables as $table){
		foreach ($permissions as $permission){
			
			$perm = "'{$permission}_".strtoupper($table['table'])."'";
			
			GenericDAO::save('seguranca', 'permissao', array('nome'=> $perm,'descricao'=>"''"));
			
			echo "Inserted $perm <br/>";
			
		}
	}
	
	
}

static function fixGeom(){
	
	$table = 'posto';
	
	$geomColumn = 'pst_geom';
	
	$geomType = 'Point';
	
	//4291
	//4326
	//900913
	
	$srid ='4326';
	
	$geometry='pst_geom';
	
	$sql = "
	BEGIN;
		update $table set $geomColumn = st_force_2d($geomColumn);

		select updategeometrysrid('$table','$geomColumn',$srid);

		select st_geometrytype($geomColumn) from $table;   

		alter table $table alter column $geomColumn type geometry($geomType,$srid);
	END;";
	
	$rs = DatabaseUtil::execute($sql);
	
	if($rs)
		echo 'Ok!';
	else 
		echo 'Error';
	
	
}
//client.txt
static function fixData() {

	$file = 'clients.txt';

	$lines = file ("$file");

	$f = 0;

	$str = null;
	
	$gid = 1;

	foreach ($lines as $line_num => $line) {
			
		$str = explode(';', $line);
	
		if($str[10] == 'FORTALEZA'){
			
			DatabaseUtil::execute("UPDATE clienten SET data = '{$str[1]}' WHERE gid = $gid");
			
			$gid++;
			
		}
		
		
		
	}

}

static function openFile() {

		//st_transform(st_setsrid(ST_GeomFromGeoJSON('{".'"type":"Point","coordinates":'. "[{$list[3]},{$list[2]}]" .'}'."'),4326), 900913)
		
		$file = 'envio_lassance.csv';

		$lines = file ("$file");

		$f = 0;

		$list = array();
		
		$name = "";
		
		DatabaseUtil::execute('delete from clienten');
		
		foreach ($lines as $line_num => $line) {
			
			$list = explode(';', $line);
			
			$name = "";
			//st_transform(st_setsrid(ST_GeomFromGeoJSON('{".'"type":"Point","coordinates":'. "[{$list[3]},{$list[2]}]" .'}'."'),4326), 900913)
			$sql = "INSERT INTO CLIENTEN(gid,name,title,type,address,longitude,latitude) VALUES({$list[0]},'{$list[1]}','{$list[2]}','{$list[3]}','{$list[4]}','{$list[5]}','{$list[6]}')";
			
			$rs = DatabaseUtil::execute($sql);
			
			if($rs){
				//echo" {$list[0]}";
				$f++;
			}
			else{
				echo "$sql Not<br/>";
			}
			
		}

		echo $f;

	}
//geocod
static function writeAddress() {
	
		$sql = "SELECT address,gid FROM client WHERE LAT IS NULL";

		$rs = DatabaseUtil::execute($sql);

		$list = array();

		$count = 0;
		
		echo "<script type='text/javascript'>var dbAddress = [";
		
		$isFirst = true;
		
		while($row = pg_fetch_row($rs)){

			if($isFirst)
				echo "{a:'{$row[0]}',gid:$row[1]}";
			else 
				echo ",{a:'{$row[0]}',gid:$row[1]}";
			
			$isFirst = false;
			
		}
		echo "];
		readDBAddress();		</script>";
		
	}
//graph
static function copyThat() {
	
	    DatabaseUtil::execute("Delete from Objeto where gid > 1");
	
		$sql = "SELECT name,address,geom FROM clienten WHERE (salesman = 3884 or salesman = 90951) and data like '%04/06/2013%'";
	
		$rs = DatabaseUtil::execute($sql);
		
		$count = 2;
	
		while($row = pg_fetch_row($rs)){

			DatabaseUtil::execute("INSERT INTO OBJETO(gid,NOME,ENDERECO,GEOM,ativo) VALUES($count,'{$row[0]}','{$row[1]}','{$row[2]}',true)");

			$count++;
		}
	
	}
	
}
?>