

var search_form = $('#search-form');
var image_input = $('#search-form input[type="file"]');
var search_btn = $('#search-btn');

image_input.change(function(e) {
	search_form.submit();
    });

search_btn.click(function(e) {
	e.preventDefault();
	image_input.click();
    });
