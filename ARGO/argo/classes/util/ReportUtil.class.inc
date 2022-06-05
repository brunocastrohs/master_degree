<?php	
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/libs/pdf/class.ezpdf.php");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/dao/GenericDAO.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/ReportConstants.class.inc");

class ReportUtil{
	
	static $additionalParams = "";

	static function processReportRequest($report,$additionalParams){
		
		//$report = base64_decode($report);
		
		ReportConstants::initReportsProperties();
		
		if(isset(ReportConstants::$reportsProperties[$report])){
		self::generateReport(ReportConstants::$reportsProperties[$report]['title'],ReportConstants::$reportsProperties[$report]['description'],ReportConstants::$reportsProperties[$report]['font'],	
			self::generateDataFromQuery(ReportConstants::$reportsProperties[$report]['tables'], 
										ReportConstants::$reportsProperties[$report]['cols'], 
										ReportConstants::$reportsProperties[$report]['params'].$additionalParams
										)
			);
			
		}
		
		
	}
	
	static function generateReport($title, $description,$font, $data){

		$pdf = new Cezpdf('a3');

		$pdf->selectFont($_SERVER['DOCUMENT_ROOT']."/ARGO/libs/pdf/fonts/Helvetica.afm");

		$pdf->addInfo(ReportConstants::$metaInformation);
		 
		if(isset($data[0])){
		
		$titles = self::generateDataTitles($data);
		
		$pdf->ezText("<b>$title</b>\n",16);
		$pdf->ezText("$description\n",12);
		$pdf->ezTable($data,$titles,'',array('fontSize' => $font,'colGap' => $font) );
		$pdf->ezText("\n\n\n",10);
		$pdf->ezText("<b>Data:</b> ".date("d/m/Y"),10);
		$pdf->ezText("<b>Hora:</b> ".date("H:i:s")."\n\n",10);
		$pdf->ezStream();
		}
		echo "<div style='font:20px bolder Arial; '>Não foram encontrados registros para compor esse relatório.</div>";
	}

	static function generateDataFromQuery($table, $cols, $params){
		
		$params .= self::$additionalParams;
		
		$records = GenericDAO::retrieveReportRecords($table, $cols, $params);
		
		$data = array();
		
		$flag = 0;
		
		$index = 1;
		
		$value_hold = "";
		
		foreach ($records as $row){
			foreach ($row as $e){
				
				if($e['column'] == 'Rua' && $value_hold == $e['value']){
					//echo "{$e['column']} => {$e['value']} || $value_hold";
					//echo "<br/>";
					$flag--;
					$index--;
					break;
				}
				
				if($e['column'] == 'Índice')
					$data[$flag][$e['column']] = $index;
				else {
					$data[$flag][$e['column']] = $e['value'];
					$value_hold = $e['value'];
				}
			}
			
			$flag++;
			$index++;
		}
		
		return $data;
		
	}
	
	static function parseRecordToData($records){
	
		$data = array();
	
		$flag = 0;
	
		foreach ($records as $row){
			foreach ($row as $e){
				$data[$flag][$e['column']] = $e['value'];
		//		echo "$flag - {$e['column']} - {$e['value']} ||";
			}
		//	echo "<br/>";
			$flag++;
		}
	
		return $data;
	
	}
	
	static function generateDataTitles($data){
		
		$titles = array();
		
		foreach (array_keys($data[0]) as $column){
			$titles[$column] = "<b>".ucwords(str_replace("_", " ", $column))."</b>";
		}
			
		return $titles;
		
	}
	
	static function generatePDF($reportTitle, $description,$font, $headers,$data){
	
		$pdf = new Cezpdf('a3');
	
		$pdf->selectFont($_SERVER['DOCUMENT_ROOT']."/ARGO/libs/pdf/fonts/Helvetica.afm");
	
		$pdf->addInfo(ReportConstants::$metaInformation);
			
		if(isset($data[0])){
	
		//	$titles = self::generateDataTitles($data);
	
			$pdf->ezText("<b>$reportTitle</b>\n",16);
			$pdf->ezText("$description\n",12);
			$pdf->ezTable($data,$headers,'',array('fontSize' => $font,'colGap' => $font) );
			$pdf->ezText("\n\n\n",10);
			$pdf->ezText("<b>Data:</b> ".date("d/m/Y"),10);
			$pdf->ezText("<b>Hora:</b> ".date("H:i:s")."\n\n",10);
			$pdf->ezStream();
		}
		echo "<div style='font:20px bolder Arial; '>Não foram encontrados registros para compor esse relatório.</div>";
	}
	
	static function generateXLS($cols,$headers,$data){

		$fp = fopen($_SERVER['DOCUMENT_ROOT'] . "/ARGO/tmp/relatorio.xls","wb");
		
		$isFirst = true;
		
		foreach ($cols as $col){
			if($isFirst)
				fwrite($fp,$headers[$col]);
			else
				fwrite($fp,";".$headers[$col]);
			
			$isFirst=false;
			
		}
		
		fwrite($fp,"\n");
		
		$isFirst = true;
		
		foreach ($data as $e){
			foreach ($cols as $col){
					
				if($isFirst){
					fwrite($fp,$e[$col]);
				}
				else{
					fwrite($fp,";".$e[$col]);
				}
				
				$isFirst=false;
			}
			$isFirst=true;
			fwrite($fp,"\n");
		}
		fclose($fp);
		
		header('Content-disposition: attachment; filename=relatorio.xls');
		header('Content-type: application/vnd.ms-excel');
		
		// The PDF source is in original.pdf
		readfile($_SERVER['DOCUMENT_ROOT'] . "/ARGO/tmp/relatorio.xls");
		
	}
	
	static function generateTXT($cols,$headers,$data){
		$fp = fopen($_SERVER['DOCUMENT_ROOT'] . "/ARGO/tmp/relatorio.txt","wb");
		$isFirst = true;
		
		foreach ($cols as $col){
			if($isFirst)
				fwrite($fp,$headers[$col]);
			else
				fwrite($fp,";".$headers[$col]);
			
			$isFirst=false;
			
		}
		
		fwrite($fp,"\n");
		
		$isFirst = true;
		
		foreach ($data as $e){
			foreach ($cols as $col){
					
				if($isFirst){
					fwrite($fp,$e[$col]);
				}
				else{
					fwrite($fp,";".$e[$col]);
				}
				
				$isFirst=false;
			}
			$isFirst=true;
			fwrite($fp,"\n");
		}
		fclose($fp);
		
		header('Content-disposition: attachment; filename=relatorio.txt');
		header('Content-type: text/plain');
		
		// The PDF source is in original.pdf
		readfile($_SERVER['DOCUMENT_ROOT'] . "/ARGO/tmp/relatorio.txt");
		
	}

}
?>