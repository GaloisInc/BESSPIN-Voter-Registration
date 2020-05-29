
document.addEventListener('submit', e => {

	document.getElementById('outerDiv').style.visibility = 'hidden';
   
  // Store reference to form to make later code easier to read
  const form = e.target;

  // Post data using the Fetch API
  fetch(form.action, {
    method: form.method,
    body: new FormData(form)
    // method: form.method,
    // body: new FormData(form)
  })  
  .then(
	function(response) {
	  if (response.status !== 200) {
	    console.log('Looks like there was a problem. Status Code: ' +
	      response.status);
	    return;
	  }

	  // Examine the text in the response
	  response.json().then(function(data) {
	    console.log(data);
	  });
	}
  )

  // Prevent the default form submit
  e.preventDefault();

});