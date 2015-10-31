<?php

require "header.inc";
include 'functions.inc';

?>
    <div class="Wrapper">
        <form method="POST" id="hide">


            <?php

            $DB_Con = null;
            funcConnectToDB( $DB_Con );

            $sql = "SELECT `File_Name` FROM `Files` WHERE `Display`=1";
            $result = mysqli_query( $DB_Con, $sql );

            if ( $result === FALSE )
            {
                printf("ERROR: %s\n", mysqli_error( $DB_Con ));
            }

            $Files = $result->num_rows-1;

            //echo '<section class="pagetitle">Delete Files</section>';
            echo '<section class="deletehead">Total Files : ' . $Files . '</section>';

            while ( $row = $result->fetch_assoc() )
            {
                if($row['File_Name'] != "stoplist.txt"){
                    $pieces = explode(".", $row['File_Name'] );

                    ?>

                    <section class="delete" id="<?php echo $pieces[0]?>"  >

                        <section class="deleteinnername">
                            <?php echo $row['File_Name']?>
                        </section>

                        <section class="deleteinner" >
                            <input method="POST" type="radio" name="fileName" value="<?php echo$row['File_Name']?>">
                        </section>

                        <div class="clear"></div>

                    </section>

                <?php
                }
            }
            ?>

            <button type="submit" class="deletebutton" target="_self">Delete</button>
        </form>

    </div>

<?
require "footer.inc";
?>