<?php 
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");

class TMPUtil{
	
static function openFileToUpdate() {
	
	/*
	 * 
	 * 
CREATE OR REPLACE VIEW clienten_900913 AS 
 SELECT clienten.gid, cliente_t.name, clienten.address, cliente_t.rota,tipo_viagem, st_transform(clienten.geom, 900913) AS geom
   FROM cliente_n clienten, cliente_t
  WHERE clienten.gid = cliente_t.gid;

delete from objeto where nome not like 'Ponto de Saída'

insert into objeto (gid,nome,endereco,rota,tipo_viagem,geom) select gid,name,address,rota,tipo_viagem,geom from clienten_900913

update objeto set ativo = true where rota like 'Rota-01' and tipo_viagem like '%Ida%'

select distinct gid_1,x,y from d order by gid_1

	 * */
	
		$file = $_SERVER['DOCUMENT_ROOT'].'/ARGO/tmp/SEPLAG-3.csv';
	
		$lines = file ("$file");
	
		$f = 1;
	
		$list = array();
		
		//DatabaseUtil::execute('delete from cliente_t');
	
		foreach ($lines as $line_num => $line) {
				
			$list = explode(';', $line);
			
			$sql = "UPDATE CLIENTE_T SET TIPO_VIAGEM = '{$list[2]}' WHERE NAME = '{$list[0]}'";
				
			$rs = DatabaseUtil::execute($sql);
				
			if($rs){
				$f++;
			}
			else{
				echo "$sql Not<br/>";
			}
				
		}
	
		echo $f;
	
}	
	
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
		//210907;RU;MON;SALAZAR;555;9584849.0;553907.0
		/*
		 * begin;
INSERT INTO cliente_t(gid,name,address) VALUES(1612,'Berto Régis Cordeiro De Oliveira','Fortaleza PASSARÉ Rua F');
INSERT INTO cliente_t(gid,name,address) VALUES(1613,'maria amelia bezerra de souza','Fortaleza Passaré RUA PROFª ESTELA COCHRANE');
INSERT INTO cliente_t(gid,name,address) VALUES(1614,'JOANA DARC MAIA FEITOSA CORREIA','Fortaleza PAPICU RUA BENTO ALBUUQERQUE, 170 ') ;
end;


SELECT CLIENTE_T GID,ADDRESS,ST_GEOMFROMeWKT('SRID=4326;POINT('||geom ||')')
FROM CLIENTE_T WHERE GEOM IS NOT NULL OR GEOM NOT LIKE ''


		 * */
	
		$file = $_SERVER['DOCUMENT_ROOT'].'/ARGO/tmp/SEPLAG-2.csv';

		$lines = file ("$file");

		$f = 1;

		$list = array();
		
		$address = "";
		
		DatabaseUtil::execute('delete from cliente_t');
		
		foreach ($lines as $line_num => $line) {
			
			$list = explode(';', $line);
			
			$name = $list[0];
			
			$address = str_replace("  ","","Fortaleza {$list[3]} {$list[2]} ");
			
			$address = str_replace(",","",$address);

			$address = explode(" ap", $address);

			$address = $address[0];
				
			
			//st_transform(st_setsrid(ST_GeomFromGeoJSON('{".'"type":"Point","coordinates":'. "[{$list[3]},{$list[2]}]" .'}'."'),4326), 900913)
			$sql = "INSERT INTO cliente_t(gid,name,address) VALUES($f,'$name','$address')";
			
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
	
		$sql = "SELECT address,gid FROM cliente_t WHERE geom is null or geom = ''";

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
				
		MapAction.readDBAddress();		</script>";
		
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