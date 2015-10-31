<?php

	//including libraries
	require "functions.inc";
	
	$fileContent = "";
	$fileName = "";

	$DB_Con = null;
	funcConnectToDB( $DB_Con );

	if ( isset( $_GET['fileName'] ) && isset( $_GET['WordSearch'] ) )
	{
		$fileName = $_GET['fileName'];
		$words = $_GET['WordSearch'];

		//echo "file name is: " . $fileName . "<br>";
		//echo "words are being searced: " . $words . "<br>";
		
		$sql = "UPDATE `Files` SET `Watches` = Watches  + 1 WHERE `File_Name`='" . $fileName . "'"; //watches +1
		echo ( $DB_Con->query( $sql ) === false ) ? 'Wrong SQL: ' . $sql : '';

		//in case that there is more that one word
		$wordsArr = explode(',', $words);
		
		//echo "Words array is :";
		//var_dump( $wordsArr );
		//echo '<br>';
		
		$filePath = './iframe_files/' . $fileName;
		//echo "File path is: " .$filePath;

        //lets say you need line 4

		$fileContent = file_get_contents( $filePath );		
		$fileContent = nl2br( $fileContent );

		foreach ($wordsArr as $value) //represented as lower case only
		{
                $fileContent = str_replace(strtolower($value), "<span class=\"new\">" . strtolower($value) . "</span>", $fileContent);
                $fileContent = str_replace(ucfirst($value), "<span class=\"new\">" . ucfirst($value) . "</span>", $fileContent); // first upper
                $fileContent = str_replace(strtoupper($value), "<span class=\"new\">" . strtoupper($value) . "</span>", $fileContent); // all upper match
                $fileContent = str_replace($value, "<span class=\"new\">" . $value . "</span>", $fileContent); // all upper match

              if(strpos($value, "%")){
                  $newJokerVal = substr($value, 0, -1);
                  $newJokerVal = '/'.$newJokerVal.'/';
                if(preg_match($newJokerVal, $fileContent, $matches)){
                    for($i = 0; $i <= count($matches) ; $i++) {
                        $fileContent = str_replace($matches[$i], "<span class=\"new\">" . $matches[$i] . "</span>", $fileContent); // all upper match
                    }
                }
              }
        }

    }
	else 
	{
		$fileContent = "ERROR: wrong URL.";
	}
echo '<div id="numOfWords"></div> <br><br>' .$fileName . ' ' . $fileContent;
?>
<!DOCTYPE html>
<html>
	<head>
	    <meta name = "viewport" content = "width=device-width, initial-scale=1">
	    <meta http-equiv = "Content-Type" content = "text/html; charset=utf-8" />
	    <link href = "css/reset.css" rel = "stylesheet" type = "text/css" />
	    <link href = "css/style.css" rel = "stylesheet" type = "text/css" />
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>


    </head>
	<body>
    
		<div class="Wrapper">

            <?echo $fileContent;?>
		</div>
        <script type="text/javascript">
            function myLocalFunction() {
                var size = $("span.new").size();
               $("#numOfWords").html(size + " Results");
            }
            $(document).ready(function() {
                myLocalFunction();
            });

        </script>

</body>
</html>