

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


$("#image-container").autobrowse({
	url: function (offset)
	    {
		return "http://api.flickr.com/services/feeds/photos_public.gne?tags=cat&tagmode=any&format=json&jsoncallback=?";
	    },
	    template: function (response)
	    {
		var markup='';
		for (var i=0; i<response.items.length; i++)
		    {
			markup+='<a href="'+response.items[i].link+'"><img class="result-image" src="'+response.items[i].media.m+'" /></a>'
			    };
		return markup;
	    },
	    loader: '<div class="loader"></div>',
	    itemsReturned: function (response) { return response.items.length; },
	    offset: 0,
	    max: 100,
	    useCache: false,
	    expiration: 1
	    }
    );
