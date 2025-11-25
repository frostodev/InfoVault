from django.contrib import admin
from .models import Course, Semester, Topic, Excercise

@admin.register(Semester)
class SemesterAdmin(admin.ModelAdmin):
    pass

@admin.register(Course)
class CourseAdmin(admin.ModelAdmin):
    list_display = ('course_name','course_code','semester_given')
    list_filter = ('semester_given',)

@admin.register(Topic)
class TopicAdmin(admin.ModelAdmin):
    list_display = ('topic_name','belonging_course','semester_given')
    list_filter = ('belonging_course','semester_given')

@admin.register(Excercise)
class QuesAdmin(admin.ModelAdmin):
    pass