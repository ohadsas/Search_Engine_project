<?php    

require "header.inc";
include 'functions.inc';

?>
			<div class="Wrapper">
				
		<!-- <form id="upload" action="upload.php" method="POST" enctype="multipart/form-data"> -->

					<fieldset>
						<input type="hidden" id="MAX_FILE_SIZE" name="MAX_FILE_SIZE" value="300000" />
						
				<div class="upload">
							<input type="file" id="fileselect" name="fileselect[]" multiple="multiple" />
						</div>
						
			</fieldset>

		<!-- </form> -->

		<div id="progress">

						</div>
					
	</div>
				
	<script src = "js/upload_functions.js"></script>

<?php
	require "footer.inc";
?>