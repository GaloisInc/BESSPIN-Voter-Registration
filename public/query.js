

$(document).ready(function(){

    // Offiical - Voter update form
    $("form#offical_update_voters").submit(function(e){
        e.preventDefault();
        console.log("Update voter clicked");
        var ids = $('input:checked[name^="vsel_"]')
            .map(function(){ return this.id}).get();
        console.log(ids);
        console.log(ids.join());
        $("#voter_ids").val(ids.join());
        $.ajax({
            url  : 'official_update_voters',
            type : 'GET',
            data : $("#official_update_voters").serialize(),
            success : function(result) {

            },
            error: function(xhr, result, text) {
                console.log("Server returned an error");
                showError();
            }
        });
    });


    // Official - query form
    $("form#official_query_form").submit(function(e){
    	e.preventDefault();

        console.log("Query Clicked");

        $.ajax({
            url : 'official_query_voters', 
            type : "GET",
            data : $("#official_query_form").serialize(),
            success : function(result) {
                console.log(result);
                // No results
                if(result["voter_q"][0] == undefined) {
                    console.log("GET from server returned no results");
                    empty_table();
                }

                
                console.log("GET Query from server succeeded");
                console.log(result);

                emptyTable();
				
                $.each(result["voter_q"], function(index, value) {
                	console.log("Adding Voter To Table");
                	console.log(value);
                	addVoterToTable(value);
                });

            },
            error: function(xhr, result, text) {
                // Handle Form Errors
                console.log("Server returned an error");

                showError();

            }
        });
    });

});


function emptyTable(){
	var div = document.getElementById("queryDataInner");

	div.innerHTML = `<table cellspacing="0" cellpadding="5" id=queryTable>
            <tr>
            	<th> Select </th>
            	<th> Voter ID </th>
                <th> First Name </th>
                <th> Last Name </th>
                <th> Residential Address </th>
                <th> Mailing Address </th>
                <th> Registered Party </th>
                <th> Birthdate </th>
                <th> Initial Reg. Time </th>
                <th> Last Reg. Update </th>
                <th> Status </th>
            </tr>
        </table>`;
}

function addVoterToTable(voter){

	var div = document.getElementById("queryTable");

	var ID = String(voter["id"]);

	var status;

	if (voter["status"] === '0'){
		status = "<td> Inactive </td>";
	}
	else{
		status = "<td> Active </td>";
	}

	div.innerHTML += "<tr>" + 
					'<td><input type="checkbox" id="' + ID + '" name="vsel_' + ID + '"></td>' +
					"<td>" + ID + "</td>" +
					"<td>"+ voter["givennames"] + "</td>" +
					"<td>" + voter["lastname"] + "</td>" +
					"<td>" + voter["resaddress"] + "</td>" +
					"<td>" + voter["mailaddress"] + "</td>" +
					"<td>" + voter["registeredparty"] + "</td>" +
					"<td>" + formatDate(voter["birthdate"]) + "</td>" +
					"<td>" + formatDate(voter["initialregtime"]) + "</td>" + 
					"<td>" + formatDate(voter["lastupdatetime"]) + "</td>" + 
					status +
					"</tr>";

}

function formatDate(epoch_time) {
    var d = new Date(0);
    d.setUTCSeconds(epoch_time);
    month = '' + (d.getMonth() + 1);
    day = '' + d.getDate();
    year = d.getFullYear();

    if (month.length < 2) 
        month = '0' + month;
    if (day.length < 2) 
        day = '0' + day;

    return [year, month, day].join('-');
}

function showError(){
	var div = document.getElementById("queryDataInner");
	div.innerHTML = "Server returned an error. See console for details";
	

}
