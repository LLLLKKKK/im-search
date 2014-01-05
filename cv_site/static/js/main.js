

var search_form = $('#search-form');
var image_input = $('#search-form input[type="file"]');
var csrf_input = $('#search-form input[name="csrfmiddlewaretoken"]');
var search_btn = $('#search-btn');
var error_label = $('#error-label');
var image_container = $('#image-container');

image_input.change(function(e) {
	formdata = new FormData();
	reader = new FileReader();
	file = image_input.get(0).files[0];
	reader.readAsDataURL(file);
	formdata.append("image", file);
	formdata.append("csrfmiddlewaretoken", csrf_input.attr('value'));
	$.ajax({
		type: "POST",
		url: "search",
		processData: false,
		contentType: false,
		data: formdata
	}).done(function(msg) {
		msg = JSON.parse(msg);
		console.log(msg);
		if (msg['error']) {
			error_label.text(msg['error']);
			error_label.parent().addClass('has-error');
		return ;
		}
			error_label.parent().removeClass('has-error');
			error_label.text('');
		if (msg['result']) {
			console.log(msg['result']);
			results = msg['result'];
			var markup='';
			if (results) {
			    for (var i=0; i< results.length; i++){
				var src = 'static/image/' + results[i]['url'] + '.jpg';
				markup+='<a href="' + src + '"><img class="result-image" src="'+ src +'" /></a>';
			    }
			}
			image_container.html(markup);
		}
	});
});

search_btn.click(function(e) {
	e.preventDefault();
	image_input.click();
    });
