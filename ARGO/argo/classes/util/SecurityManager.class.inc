<?php

session_start();

require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/HTML.class.inc");

class SecurityManager{

	
	public static function createSession($login,$pass){

		$user = GenericDAO::retrieveUser($login,$pass);

		if($user){

			$_SESSION['user'] = $user;

			HTML::reload();

		} else {

			HTML::generateJsScript("MessageManager.showMessage('".HTML::WARNING."','warning');");

		}

	}

	public static function checkSession(){

		if(isset($_SESSION['user'])){

			return true;

		}

		return false;
		
	}

	public static function checkPermission($contextPermissions){

		if(isset($_SESSION['user']) && isset($_SESSION['user']['permissions'])){
			
			$userPermissions = $_SESSION['user']['permissions'];

				foreach($userPermissions as $userPermission)
					if($userPermission == "ADMIN" || strstr($contextPermissions,$userPermission))
						return true;
			
		}
		
		return false;

	}
	

	public static function unsetSession(){

		unset($_SESSION['user']);;

		HTML::reload();
		

	}

	public static function initSig(){
		
		echo "<script type='text/javascript'>var allowFeature = null;</script>";
		
		$user = null;
		
		extract($_SESSION);
		
		if(isset($user) && isset($user['permissions']))
			echo "<script type='text/javascript'>allowFeature = true;</script>";
		
		
	}

}
?>