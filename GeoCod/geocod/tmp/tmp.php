<!DOCTYPE html>
<!--[if lt IE 7 ]><html class="ie ie6" lang="en"> <![endif]-->
<!--[if IE 7 ]><html class="ie ie7" lang="en"> <![endif]-->
<!--[if IE 8 ]><html class="ie ie8" lang="en"> <![endif]-->
<!--[if (gte IE 9)|!(IE)]><!-->
<html lang="en">
<!--<![endif]-->
<head>
<style type="text/css">
@media print {
	.gm-style .gmnoprint,.gmnoprint {
		display: none
	}
}

@media screen {
	.gm-style .gmnoscreen,.gmnoscreen {
		display: none
	}
}
</style>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta name="viewport"
	content="width=device-width, initial-scale=1, maximum-scale=1">

<!--[if lt IE 9]>
            <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->

<link rel="stylesheet" type="text/css" href="geocod/base.css">
<link rel="stylesheet" type="text/css" href="geocod/skeleton.css">
<link rel="stylesheet" type="text/css" href="geocod/layout.css">
<link rel="stylesheet" type="text/css" href="geocod/style.css">
<link rel="shortcut icon" type="image/x-icon"
	href="http://www.latlong.net/favicon.ico">

<script type="text/javascript"
	src="http://ajax.googleapis.com/ajax/libs/jquery/1.6.2/jquery.min.js"></script>
<title>Convert Address to Lat Long Geocode</title>
<meta name="description"
	content="Convert address to latitude longitude on map, also known as geocode address." />

<script type="text/javascript">
    $(document).ready(function() {
        $("#gadres").keyup(function(event){
            if(event.keyCode == 13){
                codeAddress();
            }
        });

    });

</script>
<script type="text/javascript"
	src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/GeoCod/scripts/global.js"></script>
<style>
#gadres {
	width: 320px;
	max-width: 320px;
}
</style>
</head>
<body onload="initialize()">

	<header>
		<div class="container">
			<div class="five columns">

				<a href="http://www.latlong.net/" title="Lat and long finder"> <img
					src="geocod/logo.png" alt="lat long logo">
				</a>

			</div>

			<div class="eleven columns">
				<div class="navtop">
					<ul>
						<li><a href="http://www.latlong.net/add-place.html"
							title="Add Place">Add Place</a></li>
						<li><a href="http://www.latlong.net/latest-places.html"
							title="Latest Places">Latest Places</a></li>
						<li><a href="http://www.latlong.net/countries.html"
							title="Countries List">Country List</a></li>
					</ul>
				</div>

				<div class="nav">
					<ul>
						<li><a
							href="http://www.latlong.net/convert-address-to-lat-long.html"
							title="Convert Address to Lat Long">Address → Lat Long</a></li>
						<li><a href="http://www.latlong.net/Show-Latitude-Longitude.html"
							title="Convert lat long to address map">Lat Long → Address</a>
						</li>
						<li><a href="http://www.latlong.net/lat-long-dms.html"
							title="Lat long to Degrees Minutes Seconds Converter">Lat Long
								→ DMS</a></li>
						<li><a href="http://www.latlong.net/lat-long-utm.html"
							title="Lat long to UTM Converter">Lat Long → UTM</a></li>
					</ul>
				</div>
			</div>

		</div>
	</header>
	<div class="container">
		<div class="five columns">

			<h1>Convert Address to Lat Long</h1>
			<p>
				To convert address to <strong>lat long</strong> type the address
				with city name, street name to get more accurate lat long value.
			</p>


			<div class="box latlong">

				<label for="lat">Latitude</label> <input name="lat" id="lat"
					type="text"> <label for="lng">Longitude</label> <input name="lng"
					id="lng" type="text">

			</div>

			<div class="box latlongover">
				<h3>Map Mouse Over Lat &amp; Long</h3>

				<label for="mlat">Lat</label> <input value="-3.995781" name="mlat"
					id="mlat" type="text"> <br> <label for="mlong">Long</label> <input
					value="-11.425781" name="mlong" id="mlong" type="text">
			</div>

			<div class="box latlong">
				<div class=" fb_reset" id="fb-root">
					<div
						style="position: absolute; top: -10000px; height: 0px; width: 0px;">
						<div>
							<iframe src="geocod/xd_arbiter.htm" style="border: medium none;"
								tab-index="-1" title="Facebook Cross Domain Communication Frame"
								aria-hidden="true" id="fb_xdm_frame_http"
								allowtransparency="true" name="fb_xdm_frame_http"
								frameborder="0" scrolling="no"></iframe>
							<iframe src="geocod/xd_arbiter_002.htm"
								style="border: medium none;" tab-index="-1"
								title="Facebook Cross Domain Communication Frame"
								aria-hidden="true" id="fb_xdm_frame_https"
								allowtransparency="true" name="fb_xdm_frame_https"
								frameborder="0" scrolling="no"></iframe>
						</div>
					</div>
					<div
						style="position: absolute; top: -10000px; height: 0px; width: 0px;">
						<div></div>
					</div>
				</div>
				<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/all.js#xfbml=1";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>

				<div fb-xfbml-state="rendered"
					class="fb-like fb_edge_widget_with_comment fb_iframe_widget"
					data-send="false" style="vertical-align: top;"
					data-layout="box_count" data-width="120" data-show-faces="false">
					<span style="height: 61px; width: 44px;"><iframe
							src="geocod/like.htm" class="fb_ltr"
							title="Like this content on Facebook."
							style="border: medium none; overflow: hidden; height: 61px; width: 44px;"
							name="f2fd780c5f42ddc" id="f1d10001dca377" scrolling="no"></iframe>
					</span>
				</div>

				<!-- Place this tag where you want the +1 button to render. -->
				<div id="___plusone_0"
					style="text-indent: 0px; margin: 0px; padding: 0px; background: none repeat scroll 0% 0% transparent; border-style: none; float: none; line-height: normal; font-size: 1px; vertical-align: baseline; display: inline-block; width: 50px; height: 60px;">
					<iframe title="+1" data-gapiattached="true"
						allowtransparency="true" src="geocod/fastbutton.htm"
						name="I0_1371058978997" id="I0_1371058978997" vspace="0"
						tabindex="0"
						style="position: static; top: 0px; width: 50px; margin: 0px; border-style: none; left: 0px; visibility: visible; height: 60px;"
						marginwidth="0" marginheight="0" hspace="0" frameborder="0"
						scrolling="no" width="100%"></iframe>
				</div>

				<!-- Place this tag after the last +1 button tag. -->
				<script type="text/javascript">
  (function() {
    var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
    po.src = 'https://apis.google.com/js/plusone.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
  })();
</script>
			</div>

			<p>This geo process is also known as geocode address.</p>

		</div>



		<div class="eleven columns">
			<div class="box">

				<label for="gadres">Address</label> <input id="gadres" size="24"
					onclick="this.select();" placeholder="Type address here to geocode"
					type="text">
				<button title="Find Lat &amp; Long" onclick="codeAddress();">Find
					Lat Long</button>

			</div>

			<div id="latlongmap"
				style="width: 100%; height: 518px; position: relative; background-color: rgb(229, 227, 223); overflow: hidden;">
				<div
					style="position: absolute; left: 0px; top: 0px; overflow: hidden; width: 100%; height: 100%; z-index: 0;">
					<div
						style="position: absolute; left: 0px; top: 0px; overflow: hidden; width: 100%; height: 100%; z-index: 0;">
						<div
							style="position: absolute; left: 0px; top: 0px; z-index: 1; cursor: url(&quot;http://maps.gstatic.com/mapfiles/openhand_8_8.cur&quot;), default;">
							<div
								style="position: absolute; left: 0px; top: 0px; z-index: 200;">
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 101;"></div>
							</div>
							<div
								style="position: absolute; left: 0px; top: 0px; z-index: 201;">
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 102;">
									<div
										style="position: absolute; left: 0px; top: 0px; z-index: 0;">
										<div
											style="position: absolute; left: 0px; top: 0px; z-index: 1;">
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 28px;">
												<canvas width="256" height="256"
													style="-moz-user-select: none; position: absolute; left: 0px; top: 0px; height: 256px; width: 256px;"
													draggable="false"></canvas>
											</div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 540px;"></div>
										</div>
									</div>
								</div>
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 103;">
									<div
										style="position: absolute; left: 0px; top: 0px; z-index: -1;">
										<div
											style="position: absolute; left: 0px; top: 0px; z-index: 1;">
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 28px;">
												<canvas width="256" height="256"
													style="-moz-user-select: none; position: absolute; left: 0px; top: 0px; height: 256px; width: 256px;"
													draggable="false"></canvas>
											</div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 39px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 295px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 28px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 284px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: -217px; top: 540px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: -228px;"></div>
											<div
												style="width: 256px; height: 256px; overflow: hidden; position: absolute; left: 551px; top: 540px;"></div>
										</div>
									</div>
								</div>
							</div>
							<div
								style="position: absolute; left: 0px; top: 0px; z-index: 202;">
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 104;"></div>
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 105;"></div>
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 106;"></div>
							</div>
							<div
								style="position: absolute; left: 0px; top: 0px; z-index: 100;">
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 0;">
									<div
										style="position: absolute; left: 0px; top: 0px; z-index: 1;">
										<div
											style="width: 256px; height: 256px; position: absolute; left: 39px; top: 28px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 39px; top: 284px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 295px; top: 28px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 295px; top: 284px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: -217px; top: 28px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: -217px; top: 284px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 39px; top: -228px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 39px; top: 540px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 295px; top: -228px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 295px; top: 540px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 551px; top: 28px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 551px; top: 284px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: -217px; top: -228px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: -217px; top: 540px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 551px; top: -228px;"></div>
										<div
											style="width: 256px; height: 256px; position: absolute; left: 551px; top: 540px;"></div>
									</div>
								</div>
							</div>
							<div style="position: absolute; z-index: 0; left: 0px; top: 0px;">
								<div style="overflow: hidden; width: 646px; height: 524px;">
									<img src="geocod/StaticMapService.png"
										style="width: 640px; height: 518px;">
								</div>
							</div>
							<div style="position: absolute; left: 0px; top: 0px; z-index: 0;">
								<div
									style="position: absolute; left: 0px; top: 0px; z-index: 1;">
									<div
										style="width: 256px; height: 256px; position: absolute; left: 39px; top: 28px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_006.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 39px; top: 284px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_008.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 295px; top: 28px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_005.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 295px; top: 284px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: -217px; top: 28px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_009.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: -217px; top: 284px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_002.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 39px; top: -228px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_015.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 39px; top: 540px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_010.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 295px; top: -228px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_016.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 295px; top: 540px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_004.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 551px; top: 28px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_012.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 551px; top: 284px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_003.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: -217px; top: -228px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_011.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: -217px; top: 540px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_007.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 551px; top: -228px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_013.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
									<div
										style="width: 256px; height: 256px; position: absolute; left: 551px; top: 540px; opacity: 1; transition: opacity 200ms ease-out 0s;">
										<img draggable="false" src="geocod/vt_014.png"
											style="width: 256px; height: 256px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									</div>
								</div>
							</div>
						</div>
					</div>
					<div
						style="margin: 2px 5px 2px 2px; z-index: 1000000; position: absolute; left: 0px; bottom: 0px;">
						<a title="Clique para ver esta área no Google Maps"
							href="http://maps.google.com/maps?ll=1.1,1.1&amp;z=5&amp;t=m&amp;hl=pt-BR"
							target="_blank"
							style="position: static; overflow: visible; float: none; display: inline;"><div
								style="width: 62px; height: 24px; cursor: pointer;">
								<img draggable="false" src="geocod/google_white.png"
									style="position: absolute; left: 0px; top: 0px; width: 62px; height: 24px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
							</div> </a>
					</div>
					<div
						style="z-index: 1000001; position: absolute; right: 0px; bottom: 0px;"
						class="gmnoprint">
						<div draggable="false"
							style="height: 19px; -moz-user-select: none; line-height: 19px; padding-right: 2px; padding-left: 50px; background: -moz-linear-gradient(left center, rgba(255, 255, 255, 0) 0px, rgba(255, 255, 255, 0.5) 50px ) repeat scroll 0% 0% transparent; font-family: Arial, sans-serif; font-size: 10px; color: rgb(68, 68, 68); white-space: nowrap; direction: ltr; text-align: right;">
							<a
								style="color: rgb(68, 68, 68); text-decoration: underline; cursor: pointer;">Dados
								do mapa</a><span style="display: none;">Dados cartográficos
								©2013 AfriGIS (Pty) Ltd, Basarsoft, Google, MapLink, Mapa
								GISrael, ORION-ME, basado en BCN IGN España</span><span
								style=""> - </span><a target="_blank"
								href="http://www.google.com/intl/pt-BR_US/help/terms_maps.html"
								style="color: rgb(68, 68, 68); text-decoration: underline; cursor: pointer;">Termos
								de Uso</a>
						</div>
					</div>
					<div
						style="background-color: white; padding: 15px 21px; border: 1px solid rgb(171, 171, 171); font-family: Arial, sans-serif; color: rgb(34, 34, 34); box-shadow: 0px 4px 16px rgba(0, 0, 0, 0.2); z-index: 10000002; display: none; width: 256px; height: 148px; position: absolute; left: 170px; top: 169px;">
						<div style="padding: 0px 0px 10px; font-size: 16px;">Dados do mapa</div>
						<div style="font-size: 13px;">Dados cartográficos ©2013 AfriGIS
							(Pty) Ltd, Basarsoft, Google, MapLink, Mapa GISrael, ORION-ME,
							basado en BCN IGN España</div>
						<div
							style="width: 10px; height: 10px; overflow: hidden; position: absolute; opacity: 0.7; right: 12px; top: 12px; z-index: 10000; cursor: pointer;">
							<img draggable="false" src="geocod/imgs8.png"
								style="position: absolute; left: -18px; top: -44px; width: 68px; height: 67px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
						</div>
					</div>
					<div style="position: absolute; right: 0px; bottom: 0px;"
						class="gmnoscreen">
						<div
							style="font-family: Arial, sans-serif; font-size: 10px; color: rgb(68, 68, 68); direction: ltr; text-align: right; background-color: rgb(245, 245, 245);">Dados
							cartográficos ©2013 AfriGIS (Pty) Ltd, Basarsoft, Google,
							MapLink, Mapa GISrael, ORION-ME, basado en BCN IGN España</div>
					</div>
					<div
						style="font-size: 10px; height: 17px; background-color: rgb(245, 245, 245); border: 1px solid rgb(220, 220, 220); line-height: 19px; display: none; position: absolute; right: 0px; bottom: 0px;"
						class="gmnoprint">
						<a
							href="http://maps.google.com/maps?ll=1.1,1.1&amp;z=5&amp;t=m&amp;hl=pt-BR&amp;skstate=action:mps_dialog$apiref:1&amp;output=classic"
							style="font-family: Arial, sans-serif; font-size: 85%; font-weight: bold; bottom: 1px; padding: 1px 3px; color: rgb(68, 68, 68); text-decoration: none; position: relative;"
							title="Informar erros no mapa ou nas imagens para o Google"
							target="_new">Informar erro no mapa</a>
					</div>
					<div controlheight="356" controlwidth="78" draggable="false"
						style="margin: 5px; -moz-user-select: none; position: absolute; left: 0px; top: 0px;"
						class="gmnoprint">
						<div
							style="cursor: url(&quot;http://maps.gstatic.com/mapfiles/openhand_8_8.cur&quot;), default; width: 78px; height: 78px; position: absolute; left: 0px; top: 0px;"
							controlheight="80" controlwidth="78" class="gmnoprint">
							<div
								style="width: 78px; height: 78px; position: absolute; left: 0px; top: 0px;"
								controlheight="80" controlwidth="78" class="gmnoprint">
								<div style="visibility: hidden;">
									<svg viewBox="0 0 78 78" height="78px" width="78px"
										overflow="hidden" version="1.1"
										style="position: absolute; left: 0px; top: 0px;">
										<circle stroke="#f2f4f6" fill="#f2f4f6" fill-opacity="0.2"
											stroke-width="3" r="35" cy="39" cx="39"></circle>
										<g transform="rotate(0 39 39)">
										<rect fill="#f2f4f6" stroke-width="1" stroke="#a6a6a6"
											height="11" width="12" ry="4" rx="4" y="0" x="33"></rect>
										<polyline stroke="#000" fill="#f2f4f6" stroke-width="1.5"
											stroke-linejoin="bevel"
											points="36.5,8.5 36.5,2.5 41.5,8.5 41.5,2.5"></polyline></g></svg>
								</div>
							</div>
							<div style="position: absolute; left: 10px; top: 11px;"
								controlheight="59" controlwidth="59" class="gmnoprint">
								<div
									style="width: 59px; height: 59px; overflow: hidden; position: relative;">
									<img draggable="false" src="geocod/mapcontrols3d7.png"
										style="position: absolute; left: 0px; top: 0px; width: 59px; height: 492px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
									<div title="Mover para a esquerda"
										style="position: absolute; left: 0px; top: 20px; width: 19.6667px; height: 19.6667px; cursor: pointer;"></div>
									<div title="Mover para a direita"
										style="position: absolute; left: 39px; top: 20px; width: 19.6667px; height: 19.6667px; cursor: pointer;"></div>
									<div title="Mover para cima"
										style="position: absolute; left: 20px; top: 0px; width: 19.6667px; height: 19.6667px; cursor: pointer;"></div>
									<div title="Mover para baixo"
										style="position: absolute; left: 20px; top: 39px; width: 19.6667px; height: 19.6667px; cursor: pointer;"></div>
								</div>
							</div>
						</div>
						<div controlheight="40" controlwidth="32"
							style="position: absolute; left: 23px; top: 85px;">
							<div
								style="width: 32px; height: 40px; overflow: hidden; position: absolute; left: 0px; top: 0px; visibility: hidden;">
								<img draggable="false" src="geocod/cb_scout_sprite_api_003.png"
									style="position: absolute; left: -9px; top: -102px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 1029px; height: 255px;">
							</div>
							<div
								style="width: 32px; height: 40px; overflow: hidden; position: absolute; left: 0px; top: 0px;">
								<img draggable="false" src="geocod/cb_scout_sprite_api_003.png"
									style="position: absolute; left: -107px; top: -102px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 1029px; height: 255px;">
							</div>
							<div
								style="width: 32px; height: 40px; overflow: hidden; position: absolute; left: 0px; top: 0px; visibility: hidden;">
								<img draggable="false" src="geocod/cb_scout_sprite_api_003.png"
									style="position: absolute; left: -58px; top: -102px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 1029px; height: 255px;">
							</div>
							<div
								style="width: 32px; height: 40px; overflow: hidden; position: absolute; left: 0px; top: 0px; visibility: hidden;">
								<img draggable="false" src="geocod/cb_scout_sprite_api_003.png"
									style="position: absolute; left: -205px; top: -102px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 1029px; height: 255px;">
							</div>
						</div>
						<div controlheight="0" controlwidth="0"
							style="opacity: 0.6; display: none; position: absolute;"
							class="gmnoprint">
							<img title="Girar o mapa em 90 graus"
								style="-moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; cursor: pointer; width: 22px; height: 22px;"
								draggable="false" src="geocod/rotate2.png">
						</div>
						<div controlheight="226" controlwidth="25"
							style="position: absolute; left: 27px; top: 130px;"
							class="gmnoprint">
							<div title="Aumentar o zoom"
								style="width: 23px; height: 24px; overflow: hidden; position: relative; cursor: pointer; z-index: 1;">
								<img draggable="false" src="geocod/mapcontrols3d7.png"
									style="position: absolute; left: -17px; top: -400px; width: 59px; height: 492px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
							</div>
							<div title="Clique para aplicar zoom"
								style="width: 25px; height: 178px; overflow: hidden; position: relative; cursor: pointer; top: -4px;">
								<img draggable="false" src="geocod/mapcontrols3d7.png"
									style="position: absolute; left: -17px; top: -87px; width: 59px; height: 492px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
							</div>
							<div title="Arraste para aplicar zoom"
								style="width: 21px; height: 14px; overflow: hidden; position: absolute; transition: top 0.25s ease 0s; z-index: 2; cursor: url(&quot;http://maps.gstatic.com/mapfiles/openhand_8_8.cur&quot;), default; left: 2px; top: 148px;">
								<img draggable="false" src="geocod/mapcontrols3d7.png"
									style="position: absolute; left: 0px; top: -384px; width: 59px; height: 492px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
							</div>
							<div title="Diminuir o zoom"
								style="width: 23px; height: 23px; overflow: hidden; position: relative; cursor: pointer; top: -4px; z-index: 3;">
								<img draggable="false" src="geocod/mapcontrols3d7.png"
									style="position: absolute; left: -17px; top: -361px; width: 59px; height: 492px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px;">
							</div>
						</div>
					</div>
					<div
						style="margin: 5px; z-index: 0; position: absolute; cursor: pointer; right: 0px; top: 0px;"
						class="gmnoprint">
						<div style="float: left;">
							<div title="Mostrar mapa de ruas" draggable="false"
								style="direction: ltr; overflow: hidden; text-align: center; position: relative; color: rgb(0, 0, 0); font-family: Arial, sans-serif; -moz-user-select: none; font-size: 13px; background: none repeat scroll 0% 0% rgb(255, 255, 255); padding: 1px 6px; border: 1px solid rgb(113, 123, 135); box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.4); font-weight: bold; min-width: 37px;">Mapa</div>
							<div
								style="background-color: white; z-index: -1; padding-top: 2px; border-width: 0px 1px 1px; border-style: none solid solid; border-color: -moz-use-text-color rgb(113, 123, 135) rgb(113, 123, 135); -moz-border-top-colors: none; -moz-border-right-colors: none; -moz-border-bottom-colors: none; -moz-border-left-colors: none; border-image: none; box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.4); position: absolute; left: 0px; top: 25px; text-align: left; display: none;">
								<div title="Mostrar mapa de ruas com terreno" draggable="false"
									style="color: rgb(0, 0, 0); font-family: Arial, sans-serif; -moz-user-select: none; font-size: 11px; background-color: rgb(255, 255, 255); padding: 1px 8px 3px 5px; direction: ltr; text-align: left; white-space: nowrap;">
									<span
										style="position: relative; line-height: 0; font-size: 0px; margin: 0px 5px 0px 0px; display: inline-block; background-color: rgb(255, 255, 255); border: 1px solid rgb(198, 198, 198); border-radius: 1px 1px 1px 1px; width: 13px; height: 13px; box-shadow: none; vertical-align: middle;"
										role="checkbox"><div
											style="position: absolute; left: 1px; top: -2px; width: 13px; height: 11px; overflow: hidden; display: none;">
											<img draggable="false" src="geocod/imgs8.png"
												style="position: absolute; left: -52px; top: -44px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 68px; height: 67px;">
										</div> </span><label
										style="vertical-align: middle; cursor: pointer;">Terreno</label>
								</div>
							</div>
						</div>
						<div style="float: left;">
							<div title="Mostrar imagens de satélite" draggable="false"
								style="direction: ltr; overflow: hidden; text-align: center; position: relative; color: rgb(51, 51, 51); font-family: Arial, sans-serif; -moz-user-select: none; font-size: 13px; background: none repeat scroll 0% 0% rgb(255, 255, 255); padding: 1px 6px; border-width: 1px 1px 1px 0px; border-style: solid solid solid none; border-color: rgb(113, 123, 135) rgb(113, 123, 135) rgb(113, 123, 135) -moz-use-text-color; -moz-border-top-colors: none; -moz-border-right-colors: none; -moz-border-bottom-colors: none; -moz-border-left-colors: none; border-image: none; box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.4); font-weight: normal; min-width: 50px;">Satélite</div>
							<div
								style="background-color: white; z-index: -1; padding-top: 2px; border-width: 0px 1px 1px; border-style: none solid solid; border-color: -moz-use-text-color rgb(113, 123, 135) rgb(113, 123, 135); -moz-border-top-colors: none; -moz-border-right-colors: none; -moz-border-bottom-colors: none; -moz-border-left-colors: none; border-image: none; box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.4); position: absolute; right: 0px; top: 25px; text-align: left; display: none;">
								<div title="Aumentar o zoom para a visualização de 45 graus"
									draggable="false"
									style="color: rgb(184, 184, 184); font-family: Arial, sans-serif; -moz-user-select: none; font-size: 11px; background-color: rgb(255, 255, 255); padding: 1px 8px 3px 5px; direction: ltr; text-align: left; white-space: nowrap; display: none;">
									<span
										style="position: relative; line-height: 0; font-size: 0px; margin: 0px 5px 0px 0px; display: inline-block; background-color: rgb(255, 255, 255); border: 1px solid rgb(241, 241, 241); border-radius: 1px 1px 1px 1px; width: 13px; height: 13px; box-shadow: none; vertical-align: middle;"
										role="checkbox"><div
											style="position: absolute; left: 1px; top: -2px; width: 13px; height: 11px; overflow: hidden; display: none;">
											<img draggable="false" src="geocod/imgs8.png"
												style="position: absolute; left: -52px; top: -44px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 68px; height: 67px;">
										</div> </span><label
										style="vertical-align: middle; cursor: pointer;">45°</label>
								</div>
								<div title="Mostrar imagens com nomes de rua" draggable="false"
									style="color: rgb(0, 0, 0); font-family: Arial, sans-serif; -moz-user-select: none; font-size: 11px; background-color: rgb(255, 255, 255); padding: 1px 8px 3px 5px; direction: ltr; text-align: left; white-space: nowrap;">
									<span
										style="position: relative; line-height: 0; font-size: 0px; margin: 0px 5px 0px 0px; display: inline-block; background-color: rgb(255, 255, 255); border: 1px solid rgb(198, 198, 198); border-radius: 1px 1px 1px 1px; width: 13px; height: 13px; box-shadow: none; vertical-align: middle;"
										role="checkbox"><div
											style="position: absolute; left: 1px; top: -2px; width: 13px; height: 11px; overflow: hidden;">
											<img draggable="false" src="geocod/imgs8.png"
												style="position: absolute; left: -52px; top: -44px; -moz-user-select: none; border: 0px none; padding: 0px; margin: 0px; width: 68px; height: 67px;">
										</div> </span><label
										style="vertical-align: middle; cursor: pointer;">Marcadores</label>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
			<p>You can hold and drop marker to anywhere you want to get the lat
				long of near places of your address.</p>
		</div>

	</div>

	<footer>
		<div class="container">
			<div class="sixteen columns">
				<p>© 2012-2013 www.latlong.net</p>

			</div>
		</div>
	</footer>



	<script type="text/javascript">

    var _gaq = _gaq || [];
    _gaq.push(['_setAccount', 'UA-1008641-56']);
    _gaq.push(['_trackPageview']);

    (function() {
        var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
        ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
        var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
    })();

</script>


</body>
</html>
