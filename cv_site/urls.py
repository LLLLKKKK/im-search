from django.conf.urls import patterns, include, url
from cv_site.views import index
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
                       url(r'^admin/', include(admin.site.urls)),
                       url(r'^$', index),
                       )
