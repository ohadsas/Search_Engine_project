<?php

$pageTitle = "search page";
require "header.inc";
include 'searchFunction.inc';

$DB_Con = null;
funcConnectToDB( $DB_Con );
funcUpdateDB( $DB_Con );

//ignore parse warnings
ini_set('display_errors', 0);
error_reporting(E_ERROR | E_WARNING | E_PARSE);


?>

    <section class = "Search_Container">
        <form method="post" action="index.php">
            <input type = "text" name = "query">
            <button type="submit" name="searchButton">Search!</button>
        </form>
    </section>

    <section class="Search_Results">

        <?php
        if( isset($_POST["query"]) && !empty ( $_POST["query"] ) )
        {
            $answerArr = search( $_POST["query"], $DB_Con );

            foreach ( $answerArr as $value )
            {
                echo  $value;
            }
        }
        ?>

    </section>

<?php
require "footer.inc";
?>