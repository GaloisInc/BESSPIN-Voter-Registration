function reset_form_errors() {
    $('input, select').removeClass("error");
}

$(document).ready(function(){

    // Voter registration
    $("#submit_voter_registration").on('click', function(){
        reset_form_errors()
        console.log("Submit voter registration");
        $.ajax({
            url: 'voter_register', 
            type : "GET",
            data : $("#voter_register_form").serialize(),
            success : function(result) {
                // Server returned a result
                // redirect to voter_registration_confirmation.html
                window.location.replace("voter_registration_confirmation.html");
            },
            error: function(xhr, result, text) {
                resp = xhr.responseJSON;
                console.log(resp);
                if(resp.hasOwnProperty("errors")) {
                    // Response has errors
                    $.each(resp.errors, function(index, value) {
                        console.log(index, value);
                        $('input[name ="' + index + '"]').addClass("error");
                        $('select[name ="' + index + '"]').addClass("error");  
                    });
                }
            }
        })
    });

});