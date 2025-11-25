from pyexpat import model
from django.shortcuts import redirect, render, HttpResponse
from django.views import generic
from .models import Semester, Course, Topic, Excercise
from django.contrib.auth import login,logout,authenticate
from .forms import *

def index(request):
    num_semesters = Semester.objects.all().count()
    num_courses = Course.objects.all().count()
    num_topics = Topic.objects.all().count()

    context = {
        'num_semesters': num_semesters,
        'num_courses': num_courses,
        'num_topics': num_topics,
    }
    return render(request, 'solvit/index.html', context=context)

class SemestersView(generic.ListView):
    template_name = 'solvit/semesters.html'
    context_object_name = 'semester_list'

    def get_queryset(self):
        return Semester.objects.all().order_by('semester_num')

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['course_list'] = list(Course.objects.all())
        return context

class CourseView(generic.ListView):
    template_name = 'solvit/course.html'
    model = Topic
    context_object_name = 'topic_list'
    
    def get_queryset(self, *args, **kwargs): # filtro de materias por ramo
        qs = super().get_queryset(*args, **kwargs)
        fltr = self.kwargs['slug']
        if fltr != 'all':
            qs = qs.filter(belonging_course=fltr)
        return qs

class TopicView(generic.ListView):
    model = Excercise
    template_name = 'solvit/topic.html'
    context_object_name = "ex_list"
    
    def get_queryset(self, *args, **kwargs): # filtro de materias por ramo
        qs = super().get_queryset(*args, **kwargs)
        fltr = self.kwargs['slug']
        if fltr != 'all':
            qs = qs.filter(belonging_topic=fltr)
        return qs

    '''def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['course_list'] = list(Course.objects.all())
        return context'''
    
    #MyModel.objects.order_by('?')

# ------------------------------------------------------------------------------

def registerPage(request):
    if request.user.is_authenticated:
        return redirect('/') 
    else: 
        form = createuserform()
        if request.method=='POST':
            form = createuserform(request.POST)
            if form.is_valid() :
                user=form.save()
                return redirect('login')
        context={
            'form':form,
        }
        return render(request,'solvit/register.html',context)
 
def loginPage(request):
    if request.user.is_authenticated:
        return redirect('/')
    else:
       if request.method=="POST":
        username=request.POST.get('username')
        password=request.POST.get('password')
        user=authenticate(request,username=username,password=password)
        if user is not None:
            login(request,user)
            return redirect('/')
       context={}
       return render(request,'solvit/login.html',context)
 
def logoutPage(request):
    logout(request)
    return redirect('/')