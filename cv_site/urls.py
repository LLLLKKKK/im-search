from django.conf.urls import patterns, include, url
from cv_site.views import index, search
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
                       ## url(r'^admin/', include(admin.site.urls)),
                       url(r'^$', index),
                       url(r'^search$', search),
                       )
