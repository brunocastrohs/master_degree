<?php
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/dao/PluviometroDAO.class.inc");

class PluviometroBean extends GenericBean{

	static function update($shema,$table,$object,$pk,$id){

		$hasPositionChange = PluviometroDAO::checkPluviometroPosition($object, $pk, $id);
		
		$rs = GenericDAO::update($shema,$table,$object,$pk,$id);

		if ($rs){

			if($hasPositionChange)
				self::updatePosicaoHistorico($object,$id);
			else 
				HTML::generateJsScript("MessageManager.showMessage('".HTML::UPDATED."','success');");
		
		}else 
			HTML::generateJsScript("MessageManager.showMessage('".HTML::NOT_UPDATED."','error');");
		
		
	}
	
	static function updatePosicaoHistorico($object,$id){

		$rs = PluviometroDAO::updatePosicaoHistorico($object,$id);
			
		if ($rs)
			HTML::generateJsScript("MessageManager.showMessage('".HTML::UPDATED."','success');");
		else 
			HTML::generateJsScript("MessageManager.showMessage('".HTML::NOT_UPDATED."','error');");
		
	}
	
	
}
?>
