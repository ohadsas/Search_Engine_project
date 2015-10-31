<?php

//including libraries
require "functions.inc";

$fileName = ( isset($_SERVER['HTTP_X_FILENAME'] ) ? $_SERVER['HTTP_X_FILENAME'] : false);

print_r("HTTP_X_FILENAME " . $_SERVER['HTTP_X_FILENAME']);

$folderName='./files';
echo "file name" . $fileName . " ";

$DB_Con = null;
funcConnectToDB( $DB_Con );

//var_dump(" file name" . $fileName);
if ( $fileName )
{

	//file_put_contents — Writes a string to a file
	//file_get_contents — Reads entire file into a string
	file_put_contents( $folderName .'/' . $fileName, file_get_contents('php://input') );
	echo "$fileName uploaded <br>";
	funcCheckIfFileInDB( $fileName, $DB_Con );
}
else 
{
	// form submit
	$files = $_FILES['fileselect'];

	foreach ($files['error'] as $id => $err) 
	{
		if ($err == UPLOAD_ERR_OK) 
		{
			$fileName = $files['name'][$id];
			//move_uploaded_file — Moves an uploaded file to a new location
			move_uploaded_file(	$files['tmp_name'][$id], $folderName . '/' . $fileName );
			echo "<p>File $fileName uploaded.</p>";
			funcCheckIfFileInDB( $fileName, $DB_Con );
		}
	}

}

?>