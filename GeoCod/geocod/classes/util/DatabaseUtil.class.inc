<?php
require_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/util/DatabaseConstants.class.inc");


class DatabaseUtil{

	public static $conn = null;

	public static $banco = 'coelce';

	public function openConnection(){

		$banco = self::$banco;
		$porta = '5432';
		$host = 'localhost';
		$usuario = 'postgres';
		$senha = 'postgres';

		if(self::$conn == null)
		self::$conn = pg_connect("dbname= $banco port=$porta	host = $host user = $usuario password = $senha ") or die("Nao conseguiu.");

		return self::$conn;
	} //end of function conecta

	static function execute($sqlString){

		$result = pg_query(self::openConnection(),$sqlString);

		return $result;
	}//end of executa

	public function close(){
		if(self::$conn != null)
		$boolean = pg_close(self::$conn);

		return $boolean;
	}//end of executa

}//end of class Banco
?>