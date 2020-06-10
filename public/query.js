

$(document).ready(function(){


    // Voter registration verification
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
                    console.log("no results");
                }

                // $.each(result["voter_q"][0], function(index, value) {
                //     console.log("voter-" + index + "-display", value);
                //     // convert date values
                //     if(index.includes("time") || index.includes("date")) {
                //         var d = new Date(0);
                //         d.setUTCSeconds(value);
                //         value = d.toUTCString();
                //     }
                //     if(index.includes("confidential")) {
                //         value = value ? "TRUE" : "FALSE";
                //     }
                //     $("#voter-" + index + "-display").text(value);
                // });
                console.log("success");
                console.log(result);

            },
            error: function(xhr, result, text) {
                // Handle Form Errors
                console.log("backend returned an error");
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

function addVoterToTable(/*voter*/){
	
	var voter = '{"voter_q":[{"id":101,"lastname":"lastname","givennames":"firstname","resaddress":"1234 address","mailaddress":"1234 address","registeredparty":"whig","birthdate":507859200,"idinfo":"YmxvYjE=","status":0,"initialregtime":0,"lastupdatetime":0,"confidential":0}]}';

	jsonData = JSON.parse(voter);

	var div = document.getElementById("queryTable");

	var ID = String(jsonData['voter_q'][0]["id"]);

	var status;

	if (jsonData['voter_q'][0]["status"] === '0'){
		status = "<td> Inactive </td>";
	}
	else{
		status = "<td> Active </td>";
	}

	div.innerHTML += "<tr>" + 
					'<td><input type="checkbox" id="' + ID + '" name="' + ID + '"></td>' +
					"<td>" + ID + "</td>" +
					"<td>"+ jsonData['voter_q'][0]["givennames"] + "</td>" +
					"<td>" + jsonData['voter_q'][0]["lastname"] + "</td>" +
					"<td>" + jsonData['voter_q'][0]["resaddress"] + "</td>" +
					"<td>" + jsonData['voter_q'][0]["mailaddress"] + "</td>" +
					"<td>" + jsonData['voter_q'][0]["registeredparty"] + "</td>" +
					"<td>" + formatDate(jsonData['voter_q'][0]["birthdate"]) + "</td>" +
					"<td>" + formatDate(jsonData['voter_q'][0]["initialregtime"]) + "</td>" + 
					"<td>" + formatDate(jsonData['voter_q'][0]["lastupdatetime"]) + "</td>" + 
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
