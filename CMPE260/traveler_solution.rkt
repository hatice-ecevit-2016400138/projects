#lang scheme
; 2016400138

(define (RAILWAY-CONNECTION location) (RCHELPER LOCATIONS location))
 ;this function takes the LOCATIONS database and returns a location's connections using that database
(define (RCHELPER list location)
  (cond
    ( (null? list)
    '() )
    (else
     (cond
       ( (equal? location (car(car list))) (car(cdr (cdr ( car list)))) )
       (else (RCHELPER (cdr list) location))
       ))
    ))

(define (ACCOMMODATION-COST location)
  (cond
   ( (null? (ACHELPER LOCATIONS location))
    0)
   (else
  (ACHELPER LOCATIONS location))))
 ;this function takes the LOCATIONS database and returns a location's accomodation cost using that database

(define (ACHELPER list location)
  (cond
    ( (null? list)
    '() )
    (else
     (cond
       ( (equal? location (car(car list))) (car(cdr ( car list))) )
       (else (ACHELPER (cdr list) location))
       ))
    ))

(define (INTERESTED-CITIES traveler) (ICHELPER TRAVELERS traveler))
 ;this function takes the TRAVELERS database and returns a traveler's interested cities using that database
(define (ICHELPER list traveler)
  (cond
    ( (null? list)
    '() )
    (else
     (cond
       ( (equal? traveler (car(car list))) (car(cdr ( car list))) )
       (else (ICHELPER (cdr list) traveler))
       ))
    ))

(define (INTERESTED-ACTIVITIES traveler) (IAHELPER TRAVELERS traveler))
 ;this function takes the TRAVELERS database and returns a traveler's interested activities using that database

(define (IAHELPER list traveler)
  (cond
    ( (null? list)
    '() )
    (else
     (cond
       ( (equal? traveler (car(car list))) (car(cdr(cdr ( car list)))) )
       (else (IAHELPER (cdr list) traveler))
       ))
    ))

(define (HOME traveler) (HHELPER TRAVELERS traveler))
 ;this function takes the TRAVELERS database and returns a traveler's home using that database
(define (HHELPER list traveler)
  (cond
    ( (null? list)
    '() )
    (else
     (cond
       ( (equal? traveler (car(car list))) (car(cdr(cdr (cdr( car list))))) )
       (else (HHELPER (cdr list) traveler))
       ))
    ))
(define (TRAVELER-FROM location) (TFHELPER TRAVELERS location '()))
 ;this function takes the TRAVELERS database and returns travelers from given location using that database
(define (TFHELPER list0 location list2)
  (cond
    ( (null? list0)
      list2 )
   (else
    (cond
      ( (equal? location (car(cdr(cdr(cdr(car list0)))))) (TFHELPER (cdr list0) location (append list2 (list (car(car list0))))))
      (else (TFHELPER (cdr list0) location list2))
      ))
   ))

(define (INTERESTED-IN-CITY location) (IIHELPER TRAVELERS location '()))
 ;this function takes the TRAVELERS database and returns travelers that interested in a given location using that database

(define (IIHELPER list0 location list2)
  (cond
    ( (null? list0)
      list2 )
   (else
    (cond
      ( (CONTAINSCITY location (car(cdr(car list0)))) (IIHELPER (cdr list0) location (append list2 (list (car(car list0))))))
      (else (IIHELPER (cdr list0) location list2))
      ))
   ))
;this is a basic member function for checking a list contains that item
(define (CONTAINSCITY location list)
  (cond
    ( (null? list)
      #f)
    (else
     (cond
      ( (equal? location (car list)) #t)
      (else (CONTAINSCITY location (cdr list)))
      ))
    ))

(define (INTERESTED-IN-ACTIVITY activity) (IAAHELPER TRAVELERS activity '()))
 ;this function takes the TRAVELERS database and returns travelers that likes a given activity using that database

(define (IAAHELPER list0 activity list2)
  (cond
    ( (null? list0)
      list2 )
   (else
    (cond
      ( (CONTAINSCITY activity (car(cdr(cdr(car list0))))) (IAAHELPER (cdr list0) activity (append list2 (list (car(car list0))))))
      (else (IAAHELPER (cdr list0) activity list2))
      ))
   ))
;this function takes a traveler and a location and finds the accomodation expenses using anythingincommon and aehelper functions
 (define (ACCOMMODATION-EXPENSES traveler location) 
    (cond
      ((ANYTHINGINCOMMON (car(cdr(cdr(FINDT traveler TRAVELERS)))) (car(cdr(cdr(cdr(FINDT location LOCATIONS))))))
       ( * 3 (AEHELPER traveler location (FINDT traveler TRAVELERS) (FINDT location LOCATIONS))))
      (else
     (AEHELPER traveler location (FINDT traveler TRAVELERS) (FINDT location LOCATIONS)))
      ))
;this function is for taking a line from database, specifically given item's line
 (define (FINDT traveler list)
   (cond
     ( (null? list)
       '())
     (else
      (cond
        ( (equal? traveler (car(car list))) (car list))
        (else (FINDT traveler (cdr list)))
        ))
     ))
;this function finds the accommodation cost from the database
 (define (AEHELPER traveler location list0 list1)
   (cond
     ( (null? list1)
              0)
     (else
      (cond
       (  (equal? location (car(cdr(cdr(cdr list0))))) 0)
       (else (car(cdr list1)))
       ))
     ))
;this is a basic member function, basically checking for if a list contains that item
(define (MEMBER mem list1)
  (cond
    ( (empty? list1)
      #f)
    (else
     (cond
       ((equal? mem (car list1)) #t)
       (else (MEMBER mem (cdr list1)))
       ))
    ))
;this is a function that checks if given two lists have any item in common using member function
(define ( ANYTHINGINCOMMON list0 list1)
  (cond
    ( (null? list0)
      #f)
    (else
     (cond
       ( (MEMBER (car list0) list1) #t)
       (else (ANYTHINGINCOMMON (cdr list0) list1))
       ))
    ))

(define (RAILWAY-NETWORK location) (deleteelement location (RNHELPER (RAILWAY-CONNECTION location) location '()) '()))
;this functions finds a citys railway network using bfs logic, everytime it takes the first element from list1 and append its connections to list1 and also it uses recursion
(define (RNHELPER list1 location resultlist)
  (cond
    ((null? list1)
     resultlist)
    (else
     (cond
       ((member (car list1) resultlist) (RNHELPER (cdr list1) location resultlist))
       (else
        (RNHELPER (append (RAILWAY-CONNECTION (car list1)) (cdr list1) ) location (cons (car list1) resultlist)))
    ))
 ))
;this is a function to delete a given element from the list
(define (deleteelement ele list1 list2)
  (cond
    ((null? list1)
     list2)
    (else
     (cond
       ((equal? (car list1) ele)(deleteelement ele (cdr list1) list2))
       (else
        (deleteelement ele (cdr list1) (cons (car list1) list2)))
       ))
    ))
;this function finds the travel expenses of a traveler and a location using railway network and findt
(define (TRAVEL-EXPENSES traveler location)
  (cond
    ((equal? location (car(cdr(cdr(cdr(FINDT traveler TRAVELERS)))))) 0)
    (else
     (cond
    ((member location (RAILWAY-NETWORK (car(cdr(cdr(cdr(FINDT traveler TRAVELERS))))))) (* 2 50))
    (else (* 2 100))))
    ))
;this function finds the expenses using travel expenses and accommodation expenses functions
(define (EXPENSES traveler location) (+ (TRAVEL-EXPENSES traveler location) (ACCOMMODATION-EXPENSES traveler location)))
;this funtion finds the city's that have accommodation cost between given two numbers
(define (IN-BETWEEN bigger smaller) (BIGGER (SMALLER LOCATIONS smaller '() ) bigger '()))
;this function returns cities that have smaller accommodation costs from given number
(define (SMALLER list1 smaller list2)
  (cond
    ((null? list1)
            list2)
    (else
     (cond
      ( (> smaller (car(cdr(car list1)))) (SMALLER (cdr list1) smaller (cons (car(car list1)) list2)))
      ( (= smaller (car(cdr(car list1)))) (SMALLER (cdr list1) smaller (cons (car(car list1)) list2)))
      (else
       (SMALLER (cdr list1) smaller list2))
      ))
    ))
;this function returns cities that have bigger accommodation costs from given number
(define (BIGGER list1 bigger list2)
  (cond
    ((null? list1)
            list2)
    (else
     (cond
      ( (< bigger (car(cdr(FINDT (car list1) LOCATIONS)))) (BIGGER (cdr list1) bigger (cons (car list1) list2)))
      ( (= bigger (car(cdr(FINDT (car list1) LOCATIONS)))) (BIGGER (cdr list1) bigger (cons (car list1) list2)))
      (else
       (BIGGER (cdr list1) bigger list2))
      ))
    ))