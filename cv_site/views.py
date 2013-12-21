from django.core.context_processors import csrf
from django.shortcuts import render_to_response
from forms import UploadImageForm

import os
BASE_DIR = os.path.dirname(os.path.dirname(__file__))

def index(request):
    c = {}
    c.update(csrf(request))
    return render_to_response('index.html', c)

def search(request):
    if request.method == 'POST':
        form = UploadImageForm(request.POST, request.FILES)
        if form.is_valid():
            f = request.FILES['image']
            handle_uploaded_file(f)
            return render_to_response('search.html', { 'image' : f.name })
        
    return index(request)
    
def handle_uploaded_file(f):
    with open(BASE_DIR + '/uploads/' + f.name, 'wb+') as destination:
        for chunk in f.chunks():
            destination.write(chunk)
