from django.db import models
from django.urls import reverse
import unicodedata

class Semester(models.Model):   # Modelo que representa un semestre
    semester_num = models.IntegerField(help_text='Ingresar n° del semestre', null=True)

    def __str__(self):
        return 'Semestre '+str(self.semester_num)

class Course(models.Model):     # Modelo que representa un ramo
    course_name = models.CharField(max_length=200, help_text='Ingresar nombre del ramo', null=True)
    course_code = models.SlugField(max_length=6,help_text='Ingresar código del ramo',null=True)
    semester_given = models.ForeignKey('Semester', on_delete=models.SET_NULL, null=True)

    def __str__(self):
        return self.course_name

    def get_absolute_url(self):
        return f"/solvit/course/{self.course_code}"

class Topic(models.Model):      # Modelo que representa una materia
    # Campos por Rellenar
    topic_name = models.CharField(max_length=200, help_text='Ingresar nombre de la materia', null=True)
    belonging_course = models.SlugField(max_length=6,help_text='Ingresar código del ramo',null=True)
    semester_given = models.ForeignKey('Semester', on_delete=models.SET_NULL, null=True)

    # Metadata
    class Meta:
        ordering = ['semester_given']
    
    # Métodos
    def get_absolute_url(self):
        underscorer = self.topic_name.replace(' ','_')
        validator = underscorer.replace("'","_")
        final_slug = strip_accents(validator)
        return f"/solvit/{final_slug}"

    def __str__(self):
        return self.topic_name

class Excercise(models.Model):  # Modelo que representa un ejercicio
    excercise_name = models.CharField(max_length=200,null=True)
    belonging_topic = models.SlugField(max_length=200,help_text='Ingresar nombre de la materia',null=True)
    excersise = models.CharField(max_length=200,help_text='Ingresar link de la imagen',null=True)
    excersise_video = models.CharField(max_length=200,help_text='www.youtube.com/watch?v=dQw4w9WgXcQ&ab_channel --> dQw4w9WgXcQ',null=True)
    
    def __str__(self):
        return self.excercise_name
    
    def get_topic(self):
        return self.belonging_topic.replace('_',' ')


def strip_accents(s):   # función que quita vocales con acentos
    return ''.join(c for c in unicodedata.normalize('NFD', s)
                    if unicodedata.category(c) != 'Mn')