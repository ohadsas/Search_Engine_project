<?php
	echo "Update.php";
	//including libraries
	include "functions.inc";

	$fileName = $_POST['fileName'];
	
	//echo $myid;
	//echo $tripid;
	
	$DB_Con = null;
	funcConnectToDB( $DB_Con );
	funcHideFileInDB( $fileName, $DB_Con );
	
?>