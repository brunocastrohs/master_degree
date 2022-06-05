<?php
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/Proj4Util.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/dao/GenericDAO.class.inc");


class PluviometroDAO extends GenericDAO{
	
	static function update($schema,$table,$object,$pk,$id){
	
		$sql = "UPDATE $schema.$table SET ". self::generateUpadteString($object) ." WHERE $pk = $id";
		
		$rs = DataBaseUtil::execute($sql);

		return $rs;

	}
	
	static function updatePosicaoHistorico($pluviometro,$id){

		$format = 'd/m/Y';
		
		$latLong = explode('(', $pluviometro['plu_geom']);
		
		$latLong = explode(')', $latLong[2]);
		
		$latLong = explode(' ', $latLong[0]);
		
		$latLong = Proj4Util::googeMercatorToLatLong($latLong[0], $latLong[1]);
		
		$sql = "INSERT INTO pluviometro.historico_posicao(HPO_LATITUDE,HPO_LONGITUDE,HPO_DATA_ALTERACAO,HPO_PLUVIOMETRO) VALUES('{$latLong['y']}','{$latLong['x']}','".date($format)."',{$id} )";
		
		$rs = DatabaseUtil::execute($sql);
		
		return $rs;
		
	}
	
	static function checkPluviometroPosition($pluviometro,$pk,$id){
		
		$sql = "SELECT ST_EQUALS(PLU_GEOM,{$pluviometro['plu_geom']}) FROM public.pluviometro WHERE $pk = $id";
		
		$rs = DatabaseUtil::execute($sql);
		
		$row = pg_fetch_row($rs);
		
		if($row[0] == 't')
			return false;
		
		return true;
		
	}
	
}
?>