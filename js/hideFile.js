window.onload = function() 
{
	//displayFilesForm = document.getElementById("hide")
			
	//displayFilesForm.addEventListener("submit", function(e)
	$("#hide").on('submit', function (e) 
	{
		console.log("onsubmit");
		
		e.preventDefault();
  		//var fileName = document.getElementsByName("fileName").value;
  		var fileName = $('input[name=fileName]:checked').val();
  		console.log("name is :" + fileName);
  		
	  	if( fileName != '' ) 
	  	{
		  	$.ajax(
		  		{
			    	type: 'POST',
			    	url: 'update.php',
			    	data: {'fileName': fileName},
			    	success: function(msg) 
			    	{
			      		//console.log("The message is: " + msg);
			      		window.location.href = 'remove.php';
			    	}
			  });
  		}
  		
  		
	});
};