MenuManager = {
	logo : '<img src="/ARGO/images/logo_coelce.png"/>',
	separator : '-',
	mover : '->',
	north:'',
	center:'',
	onButtonClick:function(btn) {
		 MenuManager.center.add({
			title : btn.text,
			autoScroll : true,
			closable : true,
			html : '<iframe class="page" src="/ARGO/classes/receiver/GenericReceiver.php?action=gridForm&x='+btn.x+'&y='+btn.y+'"></iframe>'
		});
	},
	onReportButtonClick:function(btn) {
		 window.open('/ARGO/classes/receiver/GenericReceiver.php?action=report&x='+btn.report);
	},
	onParamReportButtonClick:function(btn) {
		 window.open('/ARGO/classes/receiver/GenericReceiver.php?action=report&x='+btn.report+'&y='+btn.y+'&z='+btn.z);
	},
	onLoginButtonClick:function(btn) {
		
		var post = {
			action:'login',
			login:document.getElementById('login').value,
			pass:document.getElementById('pass').value
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'here',
			params : post
		});

		
	},
	onLogoutButtonClick:function(btn) {
		
		var post = {
			action:'logout'
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'here',
			params : post
		});

		
	}
	
};