#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"


 enum check_status {NOT_EXIST=0,EXIST=1,FAIL=2};

/*
*@brief grades contain a linked list with data about all the the
 students and this list contains a linked list with spesific student's grades.
*@param student_grades is list with all the students grades. 
*/
struct grades{
    struct list *students_grades;
};

 /*@param student_id is the student's id 
 * @param student_name is the student's name 
 * @param students_grades_sheet is a struct with student's grades 
 */ 
struct student_grades{
    int student_id;
    const char *student_name;
    struct list *student_grades_sheet;
};
/* @param grade is the student's grade
  *@param course_name is the name of the student's course
*/
struct course_grade{
    int grade;
    const char *course_name;
};

/* usres functions decleration*/
int student_grades_clone (void *elem, void **output);
int course_grade_clone (void *elem, void **output);
void course_grade_destroy (void *element);
void student_grades_destroy (void *element);

/* User functions */

/* this functions will be used in the user functions*/ 

/* @brief get string and copy it.
 * @param elem is a pointer to the string
 * @return out(pointer to the copy) on success, or NULL on fail*/
const char* clone_string(const char *elem) {
    const char *str=elem;
    char *out=(char*)malloc(sizeof(char)*(strlen(str)+1));
    if (!out) {
        return NULL;
    }
    strcpy(out, str);
    return out;
}

/* @brief get list and copy it.
 * @param elem is a pointer to the list
 * @return output(pointer to the copy) on success, or NULL on fail*/
struct list* clone_list(struct list *list_elm){
	if (!list_elm){
		return NULL;
	}
	struct node *list_node = list_begin(list_elm); 
	if (!list_node&&list_size((list_elm))>0){
		return NULL;
	}
    struct list *output= list_init(course_grade_clone, course_grade_destroy);
    if (!output){
    	return NULL;
    }

    void **elm= (void**) 1;
    for (int i=0;i<list_size (list_elm); i++){ 	
        if (course_grade_clone(list_get(list_node),elm)){     	
        	list_destroy(output);
        	return NULL;
        }
        if(list_push_back(output,elm)){
            list_destroy(output);
            course_grade_destroy(elm);
        	return NULL;
        }
        list_node=list_next(list_node);
    }
    return output;
}

/* @brief get string and destroy it.
 * @param a is a pointer to the string*/
void destroy_string(const char *a){
	if (a){
        free((char*)a);   
	}
}

/* @brief get struct and destroy it.
 * @param a is a pointer to the struct*/
void destroy_struct(void *a){
	if (a){
        free(a);   
	}
}

/* @brief User function, Destroy “element” in course_geade list.
 * @param element is a pointer to the list
 * always succeed */
void course_grade_destroy (void *element){
    struct course_grade *course_grade_elm = (struct course_grade*) element; 
    destroy_string(course_grade_elm->course_name);
    destroy_struct(course_grade_elm);    
}

/* @brief User function, clone “element” to “output” in course_geade list.
 * @param element is a pointer to the list
 * @param output is a pointer to the list copy
 * @Return 0 on success or 1 on fail*/
int course_grade_clone (void *elem, void **output){
	struct course_grade *course_grade_elm;
    struct course_grade **out=(struct course_grade**) output;
	const char *course_name_clone;
	int grade_clone;

	course_grade_elm=(struct course_grade*)elem;

    if (!course_grade_elm){
	     return 1;
    }

	*out=(struct course_grade*)malloc(sizeof(struct course_grade));

	course_name_clone = clone_string (course_grade_elm->course_name);
	grade_clone = course_grade_elm->grade;

	if (!(*out) || !course_name_clone) {
		/* Out of memory... */
		course_grade_destroy(*out);
		destroy_string (course_name_clone); 
		return 1;
	}

	(*out)->course_name = course_name_clone;
	(*out)->grade= grade_clone;
	course_grade_destroy (course_grade_elm); 

	return 0;
}

/* @brief User function, clone “element” to “output” in student_geades list.
 * @param element is a pointer to the list
 * @Return 0 on success or 1 on fail*/
int student_grades_clone (void *elem, void **output) {
	struct student_grades *student_grades_elm;
	struct student_grades **out=(struct student_grades**)output;
	struct list *student_grades_sheet_clone;
	const char *student_name_clone;
	int student_id_clone;

	student_grades_elm=(struct student_grades*)elem;
   
    if (!student_grades_elm){
	     return 1;
    }

	*out=(struct student_grades*)malloc(sizeof(struct student_grades));

	student_name_clone = clone_string (student_grades_elm->student_name);
	student_id_clone = student_grades_elm->student_id;
    student_grades_sheet_clone=clone_list(student_grades_elm->
    	student_grades_sheet);

	if (!(*out) || !student_name_clone || !student_grades_sheet_clone) {
		/* Out of memory... */
		student_grades_destroy(*out);
		destroy_string (student_name_clone);
		list_destroy(student_grades_sheet_clone);
		return 1;
	}

	(*out)->student_name = student_name_clone;
	(*out)->student_id = student_id_clone;
	(*out)->student_grades_sheet = student_grades_sheet_clone; 
	student_grades_destroy (student_grades_elm);

	return 0;
}

/* @brief User function, Destroy “element” in student_geades list .
 * @param element is a pointer to the list
 * always succeed */
void student_grades_destroy (void *element){
    struct student_grades *student_grades_elm = (struct student_grades*) 
    element; 
    destroy_string(student_grades_elm->student_name);
    list_destroy(student_grades_elm->student_grades_sheet);
    destroy_struct(student_grades_elm); 
}

/*the decumentetion of these functions is in the grades.h file:*/

int check_id_in_grades (struct grades *grades, int id, 
	                    struct student_grades **student){
	if (!grades){ 
	    return FAIL;
	}
	struct node *list_node = list_begin(grades->students_grades);
	if ((!list_node)&&(list_size(grades->students_grades)>0)){
		return FAIL;
	}
    while (list_node){
        *student = (struct student_grades*)list_get(list_node);
    	if (!(*student)){
    	    return FAIL; 
    	}
		if (id == (*student)->student_id){
			return EXIST;
		}
        list_node = list_next(list_node);
    }
	*student = NULL; /*student is not exist*/
	return NOT_EXIST;
}

int check_course_name (struct list *grades_sheet, const char *name){
	if (!grades_sheet){
	    return FAIL;
	}
	struct node *list_node = list_begin(grades_sheet); 
	void *elm;
	if (!list_node&&list_size(grades_sheet)>0){
		return FAIL;
	}
    while (list_node){
		elm = list_get(list_node);
    	if (!elm){
    	    return FAIL; 
    	}
		if (!strcmp(((struct course_grade*)elm)->course_name,name)){
			return EXIST;
		}
        list_node = list_next(list_node);
    }
	return NOT_EXIST;
}

struct grades* grades_init(){
	struct grades *grades = (struct grades*) malloc (sizeof(grades));
    if (!grades){
        return NULL;
	}
	    grades->students_grades = list_init (student_grades_clone,
                    student_grades_destroy);
    
    if (!(grades->students_grades)){
		grades_destroy(grades);
        return NULL;
    }
    return grades;
}


void grades_destroy(struct grades *grades){
	if (grades){
	    list_destroy(grades->students_grades); /* Always succeed */
	    destroy_struct(grades);
	}
}

int grades_add_student(struct grades *grades, const char *name, int id){
	struct student_grades *new_student;
	if (!grades){
		return 1;
	}
	if (check_id_in_grades(grades,id,&new_student)!=NOT_EXIST) 
	/*there is fail or student is exist*/
	{
		return 1;
	}
	new_student = (struct student_grades*)malloc(sizeof(struct student_grades));
	if (!new_student){
		return 1;
	}
	new_student->student_id=id;
	new_student->student_name = clone_string (name); 
	new_student->student_grades_sheet = list_init (course_grade_clone,
					course_grade_destroy);
	if ((!(new_student->student_name)&&strlen(name)>0)||
	!(new_student->student_grades_sheet)){
		student_grades_destroy(new_student); 
		return 1;
	}
	if (list_push_back(grades->students_grades,(void*)new_student)){
		student_grades_destroy(new_student); 
		return 1;
	}

	return 0;	
}

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
	if (!grades || grade<0 || grade>100){
	    return 1;
	}
	struct student_grades *student;
	if (check_id_in_grades (grades,id,&student)==EXIST && 
	check_course_name((student->student_grades_sheet),name)==NOT_EXIST){ 
	/* need to add a new grade */
		struct course_grade *new_grade = (struct course_grade*) 
	    malloc(sizeof(struct course_grade));
		if (!new_grade){
	        return 1;
		}
		new_grade->grade=grade;
		new_grade->course_name = clone_string (name);
	    if (list_push_back(student->student_grades_sheet,new_grade)){
			student_grades_destroy(new_grade); 
		    return 1;
		}
		return 0; /*succeed */
	}
	return 1;
}
				 

float grades_calc_avg(struct grades *grades, int id, char **out){
	float sum = 0;
	struct student_grades *student;
	struct course_grade *elm;
		if (!grades || check_id_in_grades (grades,id,&student)!=EXIST){
		*out = NULL;
	    return -1;
	}
	struct node *list_node = list_begin(student->student_grades_sheet);
	if (!list_node&&list_size(student->student_grades_sheet)>0){
		*out = NULL;
	    return -1;;
	}
    while (list_node){
		elm = (struct course_grade*)list_get(list_node);
    	if (!elm){
    	    *out = NULL;
			return -1; 
    	}
		sum+=elm->grade;
        list_node = list_next(list_node);
    }
	*out = (char*)clone_string(student->student_name);
	if (list_size(student->student_grades_sheet)==0){
	   return 0;
	}
	return sum/((float)list_size(student->student_grades_sheet));
}	


int grades_print_student(struct grades *grades, int id){
	int count = 1;
	struct student_grades *student;
	struct course_grade *elm;
	if (!grades || check_id_in_grades (grades,id,&student)!=EXIST){
		return 1;
	}
	struct node *list_node = list_begin(student->student_grades_sheet);
	if (!list_node&&list_size(student->student_grades_sheet)>0){
	    return 1;;
	}
	printf ("%s %d: ",student->student_name,student->student_id);
    while (list_node){
		elm = (struct course_grade*)list_get(list_node);
    	if (!elm){
			return 1; 
    	}
		if (count==1){ /*first course*/
			printf("%s %d",elm->course_name,elm->grade);
		}
		else {
			printf(", %s %d",elm->course_name,elm->grade);
		}
		count++;		
        list_node = list_next(list_node);
    }
	printf("\n"); /*new line*/
	return 0;
}

int grades_print_all(struct grades *grades){
	struct student_grades *student;
	if (!grades){
		return 1;
	}

	struct node *list_node = list_begin(grades->students_grades);
	if (!list_node&&list_size(grades->students_grades)>0){
	    return 1;
	}
    while (list_node){
		student = (struct student_grades*)list_get(list_node);
    	if (!student){
			return 1; 
    	}
		if (grades_print_student(grades, student->student_id)){ 
		/*there was a fail*/
			return 1;
		}
        list_node = list_next(list_node);
    }
	return 0;
}
