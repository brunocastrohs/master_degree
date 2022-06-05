MenuManager = {
	logo : '<img src="/GeoCod/images/logo.png"/>',
	separator : '-',
	mover : '->',
	north:'',
	center:'',
	onButtonClick:function(btn) {
		 MenuManager.center.add({
			title : btn.text,
			autoScroll : true,
			closable : true,
			html : '<iframe class="page" src="/GeoCod/classes/receiver/GenericReceiver.php?action=gridForm&x='+btn.schema+'&y='+btn.table+'"></iframe>'
		});
	},
	onReportButtonClick:function(btn) {
		 window.open('/GeoCod/classes/receiver/GenericReceiver.php?action=report&x='+btn.report);
	},
	onLoginButtonClick:function(btn) {
		
		var post = {
			action:'login',
			login:document.getElementById('login').value,
			pass:document.getElementById('pass').value
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

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

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'here',
			params : post
		});

		
	}
	
};