<?php
//including libraries
require "functions.inc";
		
/*
 *
 */
function search( $request, $DB_Con )
{
	echo '<div class="answerhead">Search Results for : '. $request .'</div>';

    $i = 0; 
    $pos = -1;
    $filesToShow = array ();
	$requestArr = array ();
	$wordsArr = array ();
	$answerArr = array();
			
        $request = str_replace( '\"' , '"', $request ); 	//get ride of escape characters
	
	$request = str_replace( "'", "", $request );		//get ride of the single quotes
		
	$request = str_replace( '"' , ' " ', $request ); 	//put spaces before and after the quotes
	
	$request = str_replace( '(' , '( ', $request ); 	//insert space after right bracket
	
	$request = str_replace( ')' , ' )', $request ); 	//insert space before left bracket
	
	$request = trim( $request ); 						//erase whitespaces from the begging and the end of the query
		
	//preg_replace — Perform a regular expression search and replace
	$request = preg_replace('!\s+!', ' ', $request); 	//get ride of double spaces 
	
    if ( !empty( $request ) )
    {			
		$requestArr = explode( " ", $request );
//		echo 'Request array is:  <br>';
//		var_dump - Displays structured information about one or more expressions that includes its type and value
//		var_dump( $requestArr );
//       	echo '<br>';
		
		$requestArr = funcRemoveQuotes( $requestArr );
//		echo 'A words array:  <br>';
//		var_dump($requestArr);
//		print_r($requestArr);
//		echo '<br>';
		
		$requestArr = funcRequestParse( $requestArr );
		//echo 'B words array:  <br>';
		//var_dump($requestArr);
		//print_r($requestArr);
		//echo '<br>';
		
		$requestArr = funcQueryDB( $DB_Con, $requestArr, $wordsArr );
		//echo 'C words array:  <br>';
		//var_dump($wordsArr);
		//print_r($requestArr);
		//echo '<br>';
		
		$requestArr = funcRemoveParenthesis( $requestArr );	
		//echo 'D words array:  <br>';
		//var_dump($wordsArr);
		//print_r($requestArr);
		//echo '<br>';
		
		$requestArr = funcParseAnswer( $requestArr );
		//echo 'E words array:  <br>';
		//var_dump($wordsArr);
		//print_r($requestArr);
		//echo '<br>';

        $answerArrWithoutDuplication = $requestArr[0];
        $requestArr = array_unique($answerArrWithoutDuplication);
		//echo 'Array to show:  <br>';
		//var_dump( $answerArr );
		//echo '<br>';
		
		$answerArr = funcPrintAnswer( $requestArr, $wordsArr, $DB_Con );
		//echo 'Files that we want to show:  <br>';
		//var_dump($answerArr);
		//print_r($answerArr);
		//echo '<br>';
		
	}

	//free result set 
	$DB_Con->close();
	//mysqli_close( $DB_Con );
    unset($_POST[ "query" ]);
 	return $answerArr;
}

/*
 * This function will iterate throught the array of
 *
 */
function funcRequestParse( $requestArr )
{
	//echo 'Request Parse func:  <br>';
	$result = 0;
	$newplace = 0;
	$tempArr = array();
	
	//get array size
	if ( isset( $requestArr ) )
	{
		$sizeOfArray = count( $requestArr );
	}
	else
	{
		$sizeOfArray = 0;
	}
	
	//add And operator before parentheses and words	
    foreach ( $requestArr as $key => $value )
	{
		switch ($value)
		{
			case '"':
				//echo 'case Quotes <br>';
				break;		
			case "And":
				//echo 'case And <br>';
				break;
			case "Or":
				//echo 'case Or <br>';
				break;
			case "Not":
                //echo 'case Not <br>';
				break;
			case ")":
				//echo 'case ) <br>';
				break;
			case "(":
				//echo 'case ( <br>';
				//echo 'before: '. $wordsArr[$key-1] . ' <br>';
				if ( $key != 0 )
				{
					if ( strcmp( $requestArr[$key-1], 'And' ) !== 0 and strcmp( $requestArr[$key-1], 'Or' ) !== 0 and strcmp( $requestArr[$key-1], 'Not' ) !== 0 )
					{
						//echo 'add And before bracket <br>';
						$tempArr[$newplace++] = "And";
					}					
				}
				break;		

			default: //case for words
				//echo 'case Word <br>';
				if ( $key != 0 and $key != $sizeOfArray )
				{
					if ( strcmp( $requestArr[$key-1], 'And' ) !== 0 and strcmp( $requestArr[$key-1], 'Or' ) !== 0  and strcmp( $requestArr[$key-1], 'Not' ) !== 0 and strcmp( $requestArr[$key-1], '(' ) !== 0)
					{
						$tempArr[$newplace++] = "And";
					}
				}
				//strtolower — Make a string lowercase
				//$value = strtolower( $value );
		}
				
		$tempArr[$newplace++] = $value;		
	}	

	return $tempArr;
}

/*
 * 
 */
function funcRemoveQuotes( $requestArr )
{
	//echo 'Remove Quotes func: <br>';
	$firstKey = -1;
	$secondKey = -1;
	$tempArr = array();
	$tempVal = "";
	
   	foreach ( $requestArr as $key => $value )
	{
		//echo "The value is: " . $value .  "<br>";
		if ( strcmp( $value, '"') === 0 &&  $firstKey == -1 )
		{
			//echo 'first found <br>';
			$firstKey = intval( $key );
		}
		elseif ( strcmp( $value, '"') === 0 && $firstKey != -1 )
		{
			//echo 'second found <br>';
			$secondKey = intval( $key );
			break;
		}
	}
	
	//echo '$firstKey: ' . $firstKey . '. $secondKey: ' . $secondKey . '<br>';
	
	if ( $firstKey != -1 && $secondKey != -1)
	{
		$tempArr = array_slice( $requestArr, 0, $firstKey );
		
		while ( $secondKey - $firstKey >= 0)
		{
			//echo 'In the while loop: ' . $requestArr[$firstKey] . ' <br>';
			$tempVal = $tempVal . " " . $requestArr[$firstKey];
			++$firstKey;
		}
		
		$tempVal = trim( $tempVal );
		//$tempVal = str_replace( ' ' , '', $tempVal ); 	//
		$request = preg_replace('!\s+!', ' ', $tempVal); 	//get ride of double spaces  // switched temp request
		
		//echo "temp value is : " . $tempVal . "<br>";
		array_push( $tempArr, $tempVal );
		
		if ( in_array ( '"', array_slice( $requestArr, $secondKey+1 ) ) )
		{
			//echo "more <br>";
			$tempArr = array_merge ( $tempArr, funcRemoveQuotes ( array_slice( $requestArr, $secondKey+1 ) ) );
		}
		else
		{
			//echo "no more <br>";
			$tempArr = array_merge( $tempArr, array_slice( $requestArr, $secondKey+1 ) );
		}
		
		return $tempArr;		
	}
	else 
	{
		return $requestArr;
	}
}

/*
 * This function will iterate throught the array of query words and operators
 * 
 */
function funcQueryDB( $DB_Con, $requestArr, &$wordsArr )
{
	//echo 'Query DataBase: <br>';
	$tempArray = array();
	$value = array();
	$star = false;
	
	$operators = array(											//operators array
	        'And'      	=> True,
	        'Or'    	=> True,
	        'Not'    	=> True,
	        '('         => True,
	        ')'      	=> True,
        );
		
	foreach ( $requestArr as $element )
	{
		if( isset( $operators[$element] ) )						//in case that the word is an operator insert it as is 
		{
			array_push($tempArray, $element); 
		}
		else 													//regular words
		{
			$row = array();
			$row[0] = "0";
			
			if ( strpos( $element, '"' ) !== false )			//found quotes operator
			{
				//echo "quote found. <br>";
				$element = str_replace( '"' , '', $element );
				$element = trim( $element );  	
			}
			elseif ( strpos( $element, '*' ) !== false)			//star operator found
			{
				//echo "star found. <br>";
				$element = str_replace( '*' , '%', $element );
				$star = true;
			}
			else 												//check stop list for existance											
			{
				//echo "check stop list for existance. <br>";
				$sql = "SELECT EXISTS (SELECT * FROM `Stop_List` WHERE (`Word_Name`)= '" . $element . "');";

				//echo 'SQL1: ' . $sql . ' <br>';

								
				$result = mysqli_query( $DB_Con, $sql );
				if ( $result === false )
				{
					printf("ERROR1: %s\n", mysqli_error( $DB_Con ));
				}
					
				$row = $result->fetch_assoc();

                //echo 'ROW is: <br>';
				//var_dump( reset($row) );
				//echo '<br>';
	
				$result->close();
			}
	
			if ( strcmp( reset($row), "0" ) === 0 )				//in case the word was not found in the stop list
			{
				array_push ( $wordsArr, $element );
				
				if ( $star )
				{
					$star = false;
					/*
					$sql = "SELECT `File_Name`
						FROM `Words`
						WHERE `Word_Name` LIKE '" . $element . "'
						AND `File_Name` NOT 
						IN (
						SELECT `File_Name`
						FROM `Files`
						WHERE `Display`='0'
						);";
					 */
					$sql = "SELECT `File_Name` 
						FROM `Files`
						WHERE `Display`='1' AND `File_Name` IN
						(SELECT `File_Name` 
						FROM `Words` 
						WHERE `Word_Name`LIKE '" . $element . "')
						ORDER BY Watches DESC;";
				}
				else 
				{
					/*
					$sql = "SELECT `File_Name`
						FROM `Words`
						WHERE `Word_Name`='" . $element . "'
						AND `File_Name` NOT 
						IN (
						SELECT `File_Name`
						FROM `Files`
						WHERE `Display`='0'
						);";
					*/	
					$sql = "SELECT `File_Name` 
						FROM `Files`
						WHERE `Display`='1' AND `File_Name` IN
						(SELECT `File_Name` 
						FROM `Words` 
						WHERE `Word_Name`='" . $element . "')
						ORDER BY Watches DESC;";
				}				
				//echo 'SQL3: ' . $sql . ' <br>';		
				
				$result = mysqli_query( $DB_Con, $sql );
			    if ( $result === FALSE )
				{
        			printf("ERROR: %s\n", mysqli_error( $DB_Con ));
    			}
				
				while ( $row = $result->fetch_assoc() )
				{
					array_push( $value, $row['File_Name']);
					//echo 'ANSWER IS:  <br>';
					//var_dump( $row );
					//echo '<br>';
				}
				
				$result->close();
							
				if ( isset( $value ) )
				{
					$sizeOfArray = count( $value );
				}
				else
				{
					$sizeOfArray = 0;
				}
				
				$sizeOfArray > 0  ? array_push($tempArray, $value) : array_push($tempArray, "Null");
			}
			else 
			{
				//echo 'Word is in the stop list !!! <br>';		
				array_push($tempArray, "Null"); 
			}

		}
		unset($value);
		$value = array();	
	}
		
	return $tempArray;
 }

/*
 * This function will iterate throught the array of strings and find parenthesis.
 * It will remove the parethesis and replace it with a subquery result.
 */
function funcRemoveParenthesis( $wordsArr )
{
	//echo '<br>Remove Parenthesis func: <br>';
	$tempArr = array();
	
	$bracketFound = true;
	
	while ( $bracketFound )
	{
		foreach ( $wordsArr as $key => $value )
		{
			$bracketFound = false;
			
			if ( strcmp( $value, '(' ) === 0 )		//start new function
			{
				//echo 'START AGAIN!!!<br>';
				$wordsArr = array_merge( array_slice( $wordsArr, 0, $key ), funcRemoveParenthesis( array_slice( $wordsArr, $key+1 ) ) );
				$bracketFound = true;
				break;
			}
			elseif ( strcmp( $value, ')' ) === 0 )	//end this function
			{
				//echo 'END THIS ONE!!! the key is: ' . $key . '<br>';
				return array_merge( funcParseAnswer( array_slice( $wordsArr, 0, $key ) ), array_slice( $wordsArr, $key+1 ) );
			}
		}	
	}

	return $wordsArr;	
}

/*
 * This function will iterate throught the array of subquery results and search for operators.
 * 
 */
function funcParseAnswer( $wordsArr )
{
	//echo 'Parse Answer funcs:   ';
		
	//get array size
	if ( isset( $wordsArr ) )
	{
		$sizeOfArray = count( $wordsArr );
	}
	else
	{
		$sizeOfArray = 0;
	}
	
	while ( $sizeOfArray > 1 )
	{
		foreach ( $wordsArr as $key => $value )
		{
			//echo 'value is : ' . $value . ' <br>';
			if (strcmp( $value, 'Or' ) === 0 )
			{
				//echo 'found Or:  <br>';
				$wordsArr = funcOrCase( $wordsArr, $key );
				break;
			}
			
			if (strcmp( $value, 'And' ) === 0 )
			{
				//echo 'found And:  <br>';
				$wordsArr = funcAndCase( $wordsArr, $key, $value );
				break;
			}
			
			if (strcmp( $value, 'Not' ) === 0 )
			{
				//echo 'found Not:  <br>';
				$wordsArr = funcNotCase( $wordsArr, $key );
				break;
			}
		}
		
		//get array size
		if ( isset( $wordsArr ) )
		{
			$sizeOfArray = count( $wordsArr );
		}
		else
		{
			$sizeOfArray = 0;
		}		
	}
	
	return $wordsArr;
}

/*
 * 
 */
function funcOrCase( $wordsArr, $pos )
{
	//echo 'Or Case func:  <br>';
	$beggining = array();
	$end = array();
	$tempArray = array();
	//$tempArray = null;
	
	if ( isset( $wordsArr ) )
	{
		$sizeOfArray = count( $wordsArr );
	}
	else
	{
		$sizeOfArray = 0;
	}
	
	//TODO: deal with first and last positions
	  
	if ( !is_array( $wordsArr[$pos-1] ) and is_array( $wordsArr[$pos+1] ) )			//previous element to Or operator is not an array while next element is an array 
	{	
		$tempArray = $wordsArr[$pos+1];												//the answer is next element 
	}
	elseif ( !is_array( $wordsArr[$pos+1] ) and  is_array( $wordsArr[$pos-1] ) )	//previous element to Or operator is an array while next element is not an array 
	{
		$tempArray = $wordsArr[$pos-1];												//the answer is previos element 
	}
	elseif ( !is_array( $wordsArr[$pos+1] ) and !is_array( $wordsArr[$pos-1] ) )	//both previos and next element are not arrays
	{
		//echo 'both are nulls <br>';
		$tempArray = 'Null';
        echo '<div class="answerhead">'. ' NO MATCHES FOUND '  .'</div>';           //the answer is null
	}
	else 																			//both previos and next element are arrays
	{
		//echo 'both are not nulls <br>';
		$tempArray = array_merge( $wordsArr[$pos-1], $wordsArr[$pos+1]);			//the answer is a combination of both next and previos arrays
	}
	
	//echo 'temp Array is: ';
	//var_dump($tempArray);
	//echo ' <br>';
	
	if ( $pos-1 != 0 )
	{
		$beggining = array_slice( $wordsArr, 0, $pos-2 );
	}
	if ( $pos+1 != $sizeOfArray )
	{
		$end = array_slice( $wordsArr, $pos+2 );
	}
	
	array_push( $beggining, $tempArray);
	return array_merge($beggining, $end);
}

/*
 * 
 */
function funcAndCase( $wordsArr, $pos, $value )
{
    //echo 'And Case func:  <br>';
    //two four seven eight two five seven eight
	$beggining = array();
	$end = array();
	$tempArray = array();

	if ( isset( $value ) )
	{
        $sizeOfArray = count( $value );
	}
	else
	{
		$sizeOfArray = 0;
	}
	
	//TODO: deal with first and last positionions
	  
	if ( !is_array( $wordsArr[$pos-1] ) ) //before AND
	{
        if($wordsArr[$pos -1] == "Null"){
            echo '<div class="answerhead">'. ' NO MATCHES FOUND '  .'</div>';
            return array();
        }
		$tempArray = $wordsArr[$pos+1]; //after AND
	}
	elseif ( !is_array( $wordsArr[$pos+1] ) )
	{
        if($wordsArr[$pos +1] == "Null"){
            echo '<div class="answerhead">'. ' NO MATCHES FOUND '  .'</div>';

            return array();
        }
		$tempArray = $wordsArr[$pos-1];
	}
	else 
	{   $tempArray = array_intersect( $wordsArr[$pos-1], $wordsArr[$pos+1] );

		foreach ( $wordsArr[$pos-1] as $key1 => $value1 )
		{
			foreach ( $wordsArr[$pos+1] as $key2 => $value2 ) {
                if (strcmp($value1, $value2) === 0)

                {
                    array_push($tempArray, $value1);
                   // echo "$tempArray[0] " . " $tempArray[1] ";
				}
			}
		}

	}
	
	if ( $pos-1 != 0 ) {

        $beggining = array_slice($wordsArr, 0, $pos - 2);
    }	if ( $pos+1 != $sizeOfArray )
	{
		$end = array_slice( $wordsArr, $pos+2 );

	}

	array_push( $beggining, $tempArray);
	return array_merge($beggining, $end);
}

/*
 * 
 */
function funcNotCase( $wordsArr, $pos )
{
	//echo "Not case function <br>";
	$beggining = array();
	$end = array();
	$tempArray = array();

    if ( isset( $value ) )
	{
		$sizeOfArray = count( $value );
	}
	else
	{
		$sizeOfArray = 0;
	}
	
	//TODO: deal with first and last positionions
	  
	if ( !is_array( $wordsArr[$pos-1] ) || !is_array( $wordsArr[$pos+1] ) )
	{
		$tempArray = $wordsArr[$pos-1];		//would be null

    }
	else 
	{
		$tempArray = array_diff( $wordsArr[$pos-1], $wordsArr[$pos+1]  );
        echo '<div class="answerhead">'. ' NO MATCHES FOUND '  .'</div>';

    }
	
	if ( $pos-1 != 0 )
	{
		$beggining = array_slice( $wordsArr, 0, $pos-2 );
	}
	if ( $pos+1 != $sizeOfArray )
	{
		$end = array_slice( $wordsArr, $pos+2 );
	}
	
	array_push( $beggining, $tempArray);
    return array_merge($beggining, $end);
}

/*
 * This function will 
 * 
 */
function funcPrintAnswer( $requestArr, $wordsArr, $DB_Con )
{
	//echo 'Print Answer func: <br>';
	$tempArr = array();	
	$words = "";
	
	//create a string of searched words from array 
	foreach($wordsArr as $key => $value)
	{
		if ( $key > 0)
		{
			$words = $words . "," . $value;
		}
		else
		{
			$words = $value;
		}
	}

    $nums = 0;
    $random = "random" ;
    $linktext = "link";

	foreach ( $requestArr as $key => $value)
	{
        //random iframe foreach taple
        $nums = $nums + 1;
        $random .= (string)$nums ;
        $linktext .= (string)$nums;

		$sql = "SELECT `Description`, `Performer`, `SongLink`
				FROM `Files`
				WHERE `File_Name`='" . $value . "';";
		//echo 'SQL: ' . $sql . ' <br>';		
		
		$result = mysqli_query( $DB_Con, $sql );
	    if ( $result === FALSE )
		{
			printf("ERROR: %s\n", mysqli_error( $DB_Con ));
		}
		
		$row = $result->fetch_assoc();

		//echo '2.ANSWER IS:  <br>';
		//var_dump( $row );
		//echo '<br>';

		$description = $row['Description'];
		$performer = $row['Performer'];
		$link = $row['SongLink'];


		$result->close();
						
		$tempArr[$key]	=
            '<div class="answer">
                <a href="print.php?fileName=' . $value . '&WordSearch=' . $words . '" target="_blank" class="'.$linktext.'"><h1 class="b">' . $value . '</h1></a>' .
//                '<h3 class="l">Description: ' . $description . '</h3>' .
                '<h3 class="l">Performer: ' . $performer . '</h3>' .
                '<iframe class="ytplayer" type="text/html" width="400" height="270" src="'.$link. '" frameborder="0"/>'.'</iframe>'.
                '<div class="descriptionDiv" style ="width:400px; height:270px; display:inline-block;"><h2>Description: ' . $description . '</h2>'.'</div>'.

            //    '<iframe class="textIframe" name="' . $random . '" width="400" height="270" target="_blank">'.'</iframe>'.
//
//            '<script>
//                        $(document).ready(function(){
//                            $(".'.$linktext.'")[0].click();
//
//                        });
//
//
//                    </script>'.
            '</div>';

	}


	return $tempArr;
}

/*
 * This function will delete old file from the data base.
 */
function funcUpdateDB( $DB_Con )
{
	//echo 'Update Data Base func: <br>';
	
	$sql = "SELECT `File_Name`,`Hided_At` FROM `Files` WHERE `Display`='0';";
	//echo 'SQL: ' . $sql . ' <br>';		
	
	$result = mysqli_query( $DB_Con, $sql );
	if ( $result === FALSE )
	{
		printf("ERROR: %s\n", mysqli_error( $DB_Con ));
	}
	
	$now = date('Y-m-d');
	//echo "now: " . $now . "<br>";

	while( $row = $result->fetch_assoc() )
	{
 		$hideDate = $row["Hided_At"];
		//echo "Hided at: " . $hideDate . "<br>"; 
		$fileName = $row["File_Name"]; 
		//echo "File name: " . $fileName . "<br>";
				
     	$diff = abs( strtotime( $now ) - strtotime( $hideDate ) );
		
		//$years = floor( $diff / (365*60*60*24) );
		//$months = floor( $diff / (30*60*60*24) );
		$days = floor( $diff / (60*60*24) );
    	//echo "days: " . $days . "<br>";
		
		if ( $days > 30 )
		{
			$sql = "DELETE FROM `Files` WHERE `File_Name` = '". $fileName . "'";
			echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';
			deleteFileFromDB( $fileName, $DB_Con );
		}
	}
}

?>

