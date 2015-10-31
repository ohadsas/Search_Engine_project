<?php
//error_reporting( _ALL );

$folderName = "./files";

/*
 * This function establishes the connection to the database.
 * $DB_Con 
 */
function funcConnectToDB( &$DB_Con )
{
	//echo 'Connect to the DB: <br>';
	$db_host = "127.0.0.1";
	$db_name = "ohad";
	$db_user = "root";
	$db_pass = "mysql";

	$DB_Con = mysqli_connect( $db_host, $db_user, $db_pass, $db_name );

	if (!$DB_Con)
	{
        echo "connection error ";
	    printf( "Connect failed: %s\n", mysqli_connect_error() );
	    exit();
	}

}

/*
 * This function will check whether the added file already exists in the data base or not.
 * 
 */
function funcCheckIfFileInDB( $fileName, $DB_Con )
{
	echo 'Check file in DB: <br>';
	$sql = "SELECT `Display` FROM `Files` WHERE `File_Name`='" . $fileName . "'";
	//echo ' SQL: ' . $sql . ' <br>';

	$result = mysqli_query( $DB_Con, $sql );
	if ( $result === false )
	{
		printf("ERROR1: %s\n", mysqli_error( $DB_Con ));
        echo "error ";
	}	
	$result = $result->fetch_assoc();
	echo "The result is: ";
	var_dump( $result );
	//echo '<br>';
	
	if ( $result != null )	//exists in db - check if visible or not
	{
		//echo 'THE file is in DB. <br>';
		$fileDisplay = $result["Display"];
		echo 'Display: ' . $fileDisplay . ' <br>';
		
	    //free result set 
    	//$result->free();
		
		if ( strcmp( $fileDisplay, "1" ) === 0 ) //the file is displayed - update DB content with file content
		{
			//echo 'Display equals to 1. <br>';
			
			//delete all the file words from data base
			deleteFileFromDB( $fileName, $DB_Con );
			
			//add all the file fords to the data base
			funcAddWordsToDB( $fileName, $DB_Con );
		}	
		else //set visibility to display
		{
			//echo 'Display equals to 0. <br>';
			
			//set Display to be 1
			$sql = "UPDATE `Files` SET `Display` = '1' WHERE `File_Name`='" . $fileName . "'";
			echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';
		}	
	}
	else //not exists - add file and the words it contains
	{
		//echo 'THE file is not in DB. <br>';
		
		//add file
		$sql = "INSERT INTO `Files` (`File_Name` ,`Display`) VALUES ('" . $fileName . "',  '1')";
	 	echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';		
		
		//add file attributes
		funcAddSongAttributes( $fileName, $DB_Con );
		
		//add words
		funcAddWordsToDB( $fileName, $DB_Con );
	}
	
	echo 'END - Check file in DB: <br>';
}

/*
 * This function will add song's attributes (like description, author,...).
 */
function funcAddSongAttributes( $fileName, $DB_Con )
{
	echo 'Add file attributes: <br>';
	
	$filePath = './files/' . $fileName;
	$index = 0;
	
	//we seach for special attributes within 5 first lines
	$handle = fopen( $filePath, "r" ); 
	if ( $handle ) 
	{
		$found = false;
		$author = "";
		$performer = "";
		$copyright = "";
		$description = "";
		$songLink = "";
		
	    while ( (($line = fgets( $handle )) !== false) && ($index < 5) ) 
	    {
	    	++$index;
			$line = str_replace( "'", "", $line );
			
			if ( strpos($line, '# Author=') !== false)
			{
				$found = true;
				$author = str_replace( '# Author=', '', $line );
				$author = trim($author);
				
		 		if ( strlen( $author ) == 0 && empty( $author ) )
				{
					$author = "NULL";
				}	
			}
			elseif ( strpos($line, '# Performer=') !== false ) 
			{
				$found = true;
				$performer = str_replace( '# Performer=', '', $line );
				$performer = trim($performer);
				
		 		if ( strlen( $performer ) == 0 && empty( $performer ) )
				{
					$performer = "NULL";
				}
			}
			elseif (  strpos($line, '# Copyright=') !== false  ) 
			{
				$found = true;
				$copyright = str_replace( '# Copyright=', '', $line );
				$copyright = trim($copyright);
				
		 		if ( strlen( $copyright ) == 0 && empty( $copyright ) )
				{
					$copyright = "NULL";
				}			
			}
			elseif (  strpos($line, '# Description=') !== false  ) 
			{
				$found = true;
				$description = str_replace( '# Description=', '', $line );
				$description = trim($description);								
			}
			elseif (  strpos($line, '# SongLink=') !== false  ) 
			{
				$found = true;
				$songLink = str_replace( '# SongLink=', '', $line );
				$songLink = trim($songLink);
				
		 		if ( strlen( $songLink ) == 0 && empty( $songLink ) )
				{
					$songLink = "NULL";
				}								
			}			
	    }
		
		//in case no description was found we will take 2 first lines of the file as a description
		if ( empty( $description ) )
		{
			rewind(  $handle );
			$index = 0;
			
			$description = "...";
			
			while ( ( ($line = fgets( $handle )) !== false ) && $index < 2 )
			{
				if ( strpos( $line, "#" ) === FALSE )
				{
					$description = "" . $description . "..." . trim( $line );
					++$index;
				}
			}
			
			$description = $description . "...";
			
	 		if ( strlen( $description ) == 0 && empty( $description ) )
			{
				$found = false;
				$description = "NULL";
			}
			else 
			{
				$found = true;
			}
			
			$description = str_replace( "'", "", $description );
		}
		
		if ( $found )
		{			
	 		$sql = "UPDATE Files SET Author='" . $author .
	 								"', Performer='" . $performer . 
	 								"', Copyright='" . $copyright .
	 								"', Description='" . $description . 
	 								"', SongLink='" . $songLink .
	 								"' WHERE File_Name='" . $fileName . "'";	
			echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql . '<br>': '';
		}
	} 
	else 
	{
	    printf("ERROR: The file was not found. \n");
	} 
	
	fclose($handle);
}

/*
 * This function will add all the words that file contains to the data base.
 */
function funcAddWordsToDB( $fileName, $DB_Con )
{
	//echo 'START Add Files to DB:\n';
	$filePath = './files/' . $fileName;
	//echo 'File path is:' . $filePath . '<br>';
	
	//get file content
   	//$contentArr = funcGetFileContent( 'files/' . $fileName );	
	$contentArr = array_map( 'trim', funcGetFileContent( $filePath ) );
	
    reset( $contentArr );
	
    foreach ( $contentArr as $word )
    {
        $word = trim( strtolower( $word ) );
		$word = str_replace( array("'", '"') , "", $word ) ;
		//echo "The words is: " . $word . "<br>";
		
		//perform in case that we have non empty word and # charactes is not present in it
        if ( strlen( $word ) > 0 && !empty( $word ) && strrpos($word, '#') === false )
        {
        	$sql = "SELECT `Appearance` FROM `Words` WHERE `Word_Name`='" . $word . "' AND `File_Name`='" . $fileName . "';";
			
            $result = $DB_Con->query( $sql );
			if ( $result === false )
			{
				printf("ERROR: %s\n", mysqli_error( $DB_Con ));
			} 					
			
		 	$result = $result->fetch_assoc();
						
		    //free result set 
	    	//$result->free();
	    	
			if ( $result != null )	//the words exists in db - add 1 to Appeacance
			{
				//echo 'THE word is in DB,';				
				$sql = "UPDATE `Words` SET `Appearance`= Appearance + 1 WHERE `Word_Name`='" . $word . "' AND `File_Name`='" . $fileName . "';";	
				echo ( $DB_Con->query( $sql ) === false ) ? 'The words is: ' . $word . 'Wrong SQL: ' . $sql : '';

			}
			else //the words does not exist in the data base - add word to data base with appearance = 1
			{
				//echo 'THE word is not in DB. \n';
				$sql = "INSERT INTO `Words` (`Word_Name`, `File_Name`) VALUES ('" . $word . "', '" . $fileName . "');";
			 	echo ( $DB_Con->query( $sql ) === false ) ? 'The words is: ' . $word . 'Wrong SQL: ' . $sql : '';		
			}
        }
     }

	//rearange the Words table
 	$sql = "ALTER TABLE `Words` ORDER BY `Word_Name`;";	
	echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql . '<br>': '';
 	//echo 'END Add Files to DB.\n';
}

/*
 * This function will hide song/file by setting the Display attribute to be 0.
 * Hided_At is the date of the operation.
 */
function funcHideFileInDB( $fileName, $DB_Con )
{
	echo 'Hide Files : <br>';
	
	$time = date('Y-m-d');
	echo $time;
	
	$sql = "SELECT `Display` FROM `Files` WHERE `File_Name`='" . $fileName . "'";
	echo ' SQL: ' . $sql . ' <br>';
			
	$result = mysqli_query( $DB_Con, $sql );
	if ( $result === false )
	{
		printf("ERROR1: %s\n", mysqli_error( $DB_Con ));
	}
	
	$result = $result->fetch_assoc();
	
	echo "The result is: ";
	var_dump( $result );
	echo '<br>';
	
	if ( $result != null )	//hide
	{
		echo 'THE file is in DB. <br>';
		$fileDisplay = $result["Display"];
		echo 'Display: ' . $fileDisplay . ' <br>';
		
		$sql = "UPDATE `Files` SET `Display` = '0', `Hided_At` = '". $time . "' WHERE `File_Name`='" . $fileName . "'";
		echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';
	}
}


/*
 * This function will delete all the words from exact file
 */
function deleteFileFromDB( $fileName, $DB_Con )
{
	echo "Delete words from DB <br>";
		
	$sql = "DELETE FROM `Words` WHERE `File_Name` = '". $fileName . "'";
	echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';
}

/*
 * This function will add all the words that file contains to the data base.
 * 
 */
function funcUploadStopList( $DB_Con )
{
	//echo 'Upload stop list to DB: <br>';
    $fileName = "Stop List.txt"; 
	
	//get file content
	$contentArr = array_map('trim', funcGetFileContent( './stop_list/' . $fileName ));
	
	//echo "Words array is: ";
	//print_r( $contentArr );
	//echo '<br>';
	
    reset( $contentArr );
	
    foreach ( $contentArr as $word )
    {
    	//echo "1.The words is: " . $word . "<br>";
        $word = trim( strtolower( $word ) );
		//echo "2.The words is: " . $word . "<br>";
        //$word = mysql_real_escape_string( $word );
        //echo "3.The words is: " . $word . "<br>";
		
        if ( strlen( $word ) > 0 && !empty( $word ) )
        {
        	$sql = "SELECT `Word_Name` FROM `Stop_List` WHERE `Word_Name`='" . $word . "';";
			
            $result = $DB_Con->query( $sql );
			if ( $result === false )
			{
				printf("ERROR: %s\n", mysqli_error( $DB_Con ));
			} 					
			
		 	$result = $result->fetch_assoc();
			
			//echo "The result is: ";
			//var_dump( $result );
			//echo '<br>';
			
		    //free result set
	    	//$result->free();
	    	
			if ( $result == null )	//exists in db - add 1 to Appeacance
			{
				//echo 'THE word is not in the Stop List. <br>';
				$sql = "INSERT INTO `Stop_List` (Word_Name) VALUES ('" . $word . "');";
			 	echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';		
			}
        }
     }
}

/*
 * This function reads the specific file and return its content as an array
 */
function funcGetFileContent( $filePath )
{
	//echo 'get file content func: <br>';
	
	//read file content and convert it to lover case
    $fileContent = strtolower( file_get_contents( $filePath ) );
	
	//get ride of the double spaces
	$fileContent = trim( preg_replace( '/\s+/', ' ', $fileContent ) );
	
	//get ride off the carrige return and enters
	$fileContent = str_replace( array("\n", "\r"), ' ', $fileContent );
	
	//reg ride of the special characters
    $fileContent = str_replace( array(',', '.', '"', ':'), "", $fileContent );
	
	//return content as array	
    return explode( ' ', $fileContent );
}

