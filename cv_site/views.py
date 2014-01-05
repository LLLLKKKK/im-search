from django.core.context_processors import csrf
from django.core import serializers
from django.http import HttpResponse
from django.shortcuts import render_to_response
from forms import UploadImageForm
import json
import os
import sys
import time
from service import SearchService

BASE_DIR = os.path.dirname(os.path.dirname(__file__))

def index(request):
    c = {}
    c.update(csrf(request))
    return render_to_response('index.html', c)

def search(request):
    if request.method != 'POST':
        return render_ajax(request, 'GET is not supported')

    form = UploadImageForm(request.POST, request.FILES)
    if not form.is_valid():
        return render_ajax(request, 'validate form failed')

    f = request.FILES['image']
    if f.content_type not in [ 'image/jpeg', 'image/jpg', 'image/bmp', 'image/png' ]:
        return render_ajax(request, 'format not supported')

    filename = handle_uploaded_file(f)    
    result = SearchService.search(filename, 0)

    return render_ajax(request, result=result)
    
def render_ajax(request, error=None, result=None):
    content = {}
    if error:
        content['error'] = error
    if result:
        content['result'] = result
    return HttpResponse(json.dumps(content))

def handle_uploaded_file(f):
    name, ext = os.path.splitext(f.name)
    filename = BASE_DIR + '/uploads/' + str(time.time()) + '.' +  ext
    with open(filename, 'wb+') as destination:
        for chunk in f.chunks():
            destination.write(chunk)

    return filename
