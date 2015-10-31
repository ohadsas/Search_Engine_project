<?php
	include 'functions.inc';
	
	$DB_Con = null;
	funcConnectToDB( $DB_Con );
 	funcUploadStopList( $DB_Con );
?>