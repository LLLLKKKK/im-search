

var search_form = $('#search-form');
var image_input = $('#search-form input[type="file"]');
var csrf_input = $('#search-form input[name="csrfmiddlewaretoken"]');
var search_btn = $('#search-btn');
var error_label = $('#error-label');

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
		}
	});
});

search_btn.click(function(e) {
	e.preventDefault();
	image_input.click();
    });


$("#image-container").autobrowse({
	url: function (offset){
		return "http://api.flickr.com/services/feeds/photos_public.gne?tags=cat&tagmode=any&format=json&jsoncallback=?";
	},
	template: function (response) {
		var markup='';
		for (var i=0; i<response.items.length; i++){
			markup+='<a href="'+response.items[i].link+'"><img class="result-image" src="'+response.items[i].media.m+'" /></a>';
		}
		return markup;
	},
	loader: '<div class="loader"></div>',
	itemsReturned: function (response) { return response.items.length; },
	offset: 0,
	max: 100,
	useCache: false,
	expiration: 1
});
