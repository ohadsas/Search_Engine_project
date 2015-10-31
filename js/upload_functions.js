//coloring the wanted removed file with red background
$(document).ready(function(){
    $('input[type=radio]').click(function()
    {
        var res = this.value.split(".");
        $(".delete").css("background-color", "#32A851");
        $("#" + res[0]).css("background-color", "#EC2427");
    });
});
