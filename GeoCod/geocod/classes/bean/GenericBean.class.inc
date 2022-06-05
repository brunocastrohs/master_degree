<?php
require_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/util/HTML.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/libs/PHPLiveX/PHPLiveX.php");
require_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/dao/GenericDAO.class.inc");

header("Content-Type: text/html; charset=UTF-8");

class GenericBean {

	static $ajax;

	static function retrieveYouMean($param){

		echo soundex('IPE');

	}

	static function lucian(){

		header("Content-Type: text/html; charset=UTF-8");

		$sql = "SELECT NAME,GID FROM PHONETIC_VIA";

		$rs = DatabaseUtil::execute($sql);

		$str = '';

		$count = 0;

		$words = '';

		while($row = pg_fetch_row($rs)){
				
			//$str = mb_convert_encoding($row[0],'UTF-8','ISO-8859-1');
				
			echo $row[0];
				
			$words = explode(" ", $row[0]);
				
			$str = '';
				
			foreach ($words as $word)
			$str .= soundex($word)." ";
				
			if(DatabaseUtil::execute("UPDATE PHONETIC_VIA SET phonetic_key = '$str' WHERE GID = {$row[1]}" ))
			echo "($str)Updated!<br/>";
			else
			echo "NOT Updated!!!!!!!!!!!!<br/>";

			$count++;
		}

		echo $count;

	}

	static function retrieveLatLong($param){

		$rows = GenericDAO::retrieveLatLong($param);

		HTML::generateJsScript("MapAction.setCenter('".implode(' ', $rows)."')");

	}

	static function retrieveGISRecords($param){

		$rows = GenericDAO::retrieveGISRecords($param);
			
		$htmlTable = "";
			
		if(isset($rows[0])){
			$tableSize = count($rows[0]);

			$size = 100/$tableSize;

			$htmlTable .="<div id='size' style='display:none;'>$tableSize</div><TABLE class='gisRecords' style='overflow:auto' cellspacing='0' ><TR><THEAD>";

			foreach($rows[0] as $col){
				$htmlTable .= HTML::createColumn($col['column'],$size,true);
			}

			$htmlTable .="</THEAD></TR><TBODY>";

			foreach($rows as $row){
					
				$htmlTable .= '<TR>';
					
				foreach ($row as $col){

					$htmlTable .= HTML::createColumn($col['value'],$size,false);

				}
					
				$htmlTable .= '</TR>';
					
			}

			$htmlTable .= "</TBODY></TABLE>";
		}

		echo $htmlTable;

	}

	static function retrievePhoneGISRecords($param){

		$rows = GenericDAO::retrievePhoneGISRecords($param);
			
		$htmlTable = "";
			
		if(isset($rows[0])){
			$tableSize = count($rows[0]);

			$size = 100/$tableSize;

			$htmlTable .="<div id='size' style='display:none;'>$tableSize</div><TABLE class='gisRecords' style='overflow:auto' cellspacing='0' ><TR><THEAD>";

			foreach($rows[0] as $col){
				$htmlTable .= HTML::createColumn($col['column'],$size,true);
			}

			$htmlTable .="</THEAD></TR><TBODY>";

			foreach($rows as $row){
					
				$htmlTable .= '<TR>';
					
				foreach ($row as $col){

					$htmlTable .= HTML::createColumn($col['value'],$size,false);

				}
					
				$htmlTable .= '</TR>';
					
			}

			$htmlTable .= "</TBODY></TABLE>";
		}

		echo $htmlTable;

	}

	static function retrieveLevenshteinGISRecords($param){

		$rows = GenericDAO::retrieveLevenshteinGISRecords($param);
			
		$htmlTable = "";
			
		if(isset($rows[0])){
			$tableSize = count($rows[0]);

			$size = 100/$tableSize;

			$htmlTable .="<div id='size' style='display:none;'>$tableSize</div><TABLE class='gisRecords' style='overflow:auto' cellspacing='0' ><TR><THEAD>";

			foreach($rows[0] as $col){
				$htmlTable .= HTML::createColumn($col['column'],$size,true);
			}

			$htmlTable .="</THEAD></TR><TBODY>";

			foreach($rows as $row){
					
				$htmlTable .= '<TR>';
					
				foreach ($row as $col){

					$htmlTable .= HTML::createColumn($col['value'],$size,false);

				}
					
				$htmlTable .= '</TR>';
					
			}

			$htmlTable .= "</TBODY></TABLE>";
		}

		echo $htmlTable;

	}

	static function openFile() {

		//header("Content-Type: text/html; charset=ISO-8859-1");

		//$file = '23041032500.TXT';

		$file = '23044000508.TXT';

		//$file = 'Amostra_Domicilios_23.txt';

		//$file = 'Amostra_Pessoas_23.txt';

		$lines = file ($_SERVER['DOCUMENT_ROOT']."/GeoCod/tmp/$file");

		$str = '';

		$flag = 0;

		foreach ($lines as $line_num => $line) {
			echo "$line($flag)<br/>";
			$flag++;
			//if($flag == 500)
			//break;
		}

		/*
		 $handle = fopen("Amostra_Domicilios_23.txt", "rb");
		 $contents = '';
		 $flag =0;
		 while (!feof($handle)) {
		 $contents = fread($handle, 8192);
		 echo "$contents<br/>";
		 $flag++;
		 if($flag == 1)
		 break;
		 }
		 fclose($handle);
		 */
	}

	static function openXLS() {

		header('Content-Type: text/html; charset=iso-8859-1');
		require_once $_SERVER['DOCUMENT_ROOT'].'/jxlrwtest/Excel/reader.php';

		$data = new Spreadsheet_Excel_Reader();

		$data->setOutputEncoding('ISO-8859-1');

		$data->read($_SERVER['DOCUMENT_ROOT'].'/jxlrwtest/Excel/teaser.xls');

		$size = ($data->sheets[0]['numCols']) *100;

		$colSize = 100 + $size/$data->sheets[0]['numCols'];

		$size += 450;

		$flag = 0;

		echo "<div style='width:{$size}px '>";

		error_reporting(E_ALL ^ E_NOTICE);

		$endereco = '';

		$bairro = '';

		$cidade = '';

		for ($i = 2; $i <= $data->sheets[0]['numRows']; $i++) {
			for ($j = 1; $j <= $data->sheets[0]['numCols']; $j++) {
				//echo "<div style='width:{$colSize}px; float:left; border:1px solid black; height:50px;'>".$data->sheets[0]['cells'][$i][$j]."</div>";
				if($j == 1)
					$endereco = $data->sheets[0]['cells'][$i][$j];
				else if($j == 2)
					$bairro = $data->sheets[0]['cells'][$i][$j];
				else if($j == 3)
					$cidade = $data->sheets[0]['cells'][$i][$j];
			}
				
			if(DatabaseUtil::execute("INSERT INTO SAOPAULO.CLIENTE(ENDERECO,BAIRRO,CIDADE,UF) VALUES('$endereco','$bairro','$cidade','SP')")){
				echo "<div style='width:100%; float:left; border:1px solid black; height:50px;'>YES $endereco</div>";
			}else echo "<div style='width:100%; float:left; border:1px solid black; height:50px;'>No!! $endereco</div>";
				
		}
		echo '</div>';


	}

	static function cmd(){

		//echo exec($_SERVER['DOCUMENT_ROOT']."/GeoCod/tmp/bat.bat");

		//echo exec('C:/Program Files/Aspell/bin/aspell.exe -v');

		//passthru('C:\Program Files\Aspell\bin\aspell.exe -v');
		//passthru ('');

		/*
		 header("Content-Type: text/html; charset=utf-8");

		 $str = 'Ruminß, Sumo';
		 echo $str;
		 echo "<br/>";
		 echo mb_detect_encoding($str);
		 echo "<br/>";
		 echo mb_convert_encoding($str, 'UTF-8','CP1252');
		 echo "<br/>";
		 echo mb_convert_encoding($str, 'UTF-8','UTF-8');
		 echo "<br/>";
		 echo mb_convert_encoding($str, 'UTF-8','ISO-8859-1');
		 */


		exec("aspell.exe -v >aspell.txt");


	}

}
?>