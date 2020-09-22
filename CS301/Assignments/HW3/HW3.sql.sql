-- Homework #3: SQL Queries on the Company Database
-- 1. a)
SELECT UNIQUE
    ssn, salary
FROM 
    employee, department
WHERE 
    salary < 30000
OR
    (employee.dno = department.dnumber 
AND 
    department.dname = 'Administration');
        
-- 1. b)
SELECT
    ssn, salary
FROM 
    employee JOIN department ON dno = dnumber
WHERE 
    salary < 30000
OR
    department.dname = 'Administration';
    
-- 1. c)
SELECT
    ssn, salary
FROM 
    employee
WHERE 
    salary < 30000
OR
    dno IN (SELECT dnumber FROM department WHERE dname = 'Administration');
        
-- 1. d)
SELECT
    ssn, salary
FROM 
    employee
WHERE 
    salary < 30000
    OR
    dno = ANY
        (SELECT dnumber FROM department WHERE dname = 'Administration');
        
-- 1. e)
SELECT
    ssn, salary
FROM 
    employee
WHERE EXISTS (
    SELECT 
        * 
    FROM 
        department 
    WHERE 
        department.dnumber = employee.dno
    AND ( 
            salary < 30000
            OR
            department.dname = 'Administration'
        )
    );
        
-- 1. f)
SELECT
    ssn, salary
FROM 
    (
    SELECT 
        ssn, salary
    FROM 
        employee JOIN department ON employee.dno = department.dnumber
    WHERE
        salary < 30000
    OR
        department.dname = 'Administration'
    );
    
-- 2. 
SELECT
    project.pnumber AS "Project #", 
    department.dnumber AS "Department #", 
    employee.lname AS "Manager",
    employee.address AS "Manager Address",
    employee.bdate AS "Manager Birthdate"
FROM 
    project
    INNER JOIN department ON project.dnum = department.dnumber
    INNER JOIN employee ON department.mgrssn = employee.ssn
WHERE
    project.plocation = 'Stafford';
    
-- 3. a)
SELECT 
    dname, dlocation, plocation, pnumber
FROM
    department 
    INNER JOIN dept_locations ON
        department.dnumber = dept_locations.dnumber
    INNER JOIN project ON 
        department.dnumber= project.dnum
WHERE
    project.plocation = dept_locations.dlocation;
    

-- 3. b)
SELECT 
    dname, dlocation, plocation, pnumber
FROM
    department 
    NATURAL JOIN dept_locations
    INNER JOIN project ON 
        dnumber= dnum
WHERE
    plocation = dlocation;

-- 4. a)
SELECT 
    essn AS "Social Security #"
FROM
    works_on
WHERE
    hours < 10
INTERSECT
SELECT 
    essn
FROM 
    works_on
WHERE
    hours > 30;

-- 4. b) 
SELECT UNIQUE
    essn AS "Social Security #"
FROM 
    works_on
WHERE essn IN (
    SELECT 
        ssn
    FROM 
        employee INNER JOIN works_on ON essn = ssn
    WHERE 
        hours < 10
    )
    AND essn IN (
    SELECT 
        ssn
    FROM 
        employee INNER JOIN works_on ON essn = ssn
    WHERE 
        hours > 30
    );

-- 4. b) Alternate (correlated subquery)
SELECT UNIQUE
    ssn AS "Social Security #"
FROM 
    employee e
WHERE EXISTS (
    SELECT 
        essn
    FROM 
        works_on
    WHERE 
        e.ssn = essn AND hours < 10
    )
    AND EXISTS (
    SELECT 
        essn
    FROM 
        works_on
    WHERE 
       e.ssn = essn AND hours > 30
    );    

-- 4. c)
SELECT 
    essn AS "Social Security #"
FROM
    works_on
WHERE
    hours < 10
UNION
SELECT 
    essn
FROM 
    works_on
WHERE
    hours > 30;

-- 4. d)
SELECT UNIQUE
    essn AS "Social Security #"
FROM 
    works_on
WHERE
    hours < 10 OR hours > 30;

-- 5. a) Shouldn't get: Franklin T Wong, Jennifer S Wallace, John Smith
SELECT UNIQUE
    ssn AS "SSN", CONCAT(CONCAT(fname, ' '), lname) AS "Name"
FROM 
    employee
WHERE NOT EXISTS (
    SELECT essn 
    FROM dependent WHERE ssn = essn)
UNION
SELECT UNIQUE
    ssn AS "SSN", CONCAT(CONCAT(fname, ' '), lname) AS "Name"
FROM 
    employee
WHERE NOT EXISTS (
    SELECT essn
    FROM dependent WHERE ssn = essn AND relationship = 'Spouse' ); 

-- 5. b) Shouldn't get: Franklin T Wong, Jennifer S Wallace, John Smith
SELECT UNIQUE
    ssn AS "SSN", CONCAT(CONCAT(fname, ' '), lname) AS "Name"
FROM 
    employee
WHERE NOT EXISTS (
    SELECT * FROM dependent WHERE ssn = essn)
OR NOT EXISTS (
    SELECT * FROM dependent WHERE ssn = essn AND relationship = 'Spouse' );

-- 6. 
SELECT 
    fname, lname, ((SYSDATE - TO_DATE(bdate, 'yyyy-mm-dd'))/365) AS age 
FROM 
    employee
ORDER BY 
    age DESC;

-- 7. 
SELECT 
    AVG(((SYSDATE - TO_DATE(bdate, 'yyyy-mm-dd'))/365)) AS "Average Employee Age"
FROM 
    employee;
    
-- 8. a)
SELECT 
    fname, lname, COUNT(*) AS numProjects
FROM 
    employee e INNER JOIN works_on w ON 
        e.ssn = w.essn
GROUP BY 
    fname, lname;
    
-- 8. b)
SELECT * FROM (
    SELECT 
        fname, lname, COUNT(w.essn) AS numProjects
    FROM 
        employee e INNER JOIN works_on w ON 
            e.ssn = w.essn
    GROUP BY 
        fname, lname
    )
WHERE
    numProjects > 2;
    
-- 8. c)
SELECT 
    fname, lname, COUNT(w.essn) AS numProjects
FROM 
    employee e LEFT JOIN works_on w ON 
        e.ssn = w.essn
GROUP BY 
    fname, lname;

-- 9. a)
SELECT 
    essn, pname, hours
FROM 
    works_on INNER JOIN project ON works_on.pno = project.pnumber
WHERE
    hours > (SELECT AVG(hours) FROM works_on);
    
-- 9. b)
SELECT
    pname, AVG(hours) AS averageHours
FROM
    works_on INNER JOIN project ON works_on.pno = project.pnumber
GROUP BY 
    pname;

-- 9. c)
SELECT 
    pname, averageHours
FROM 
    (
    SELECT
        pname, AVG(hours) AS averageHours
    FROM
        works_on RIGHT JOIN project ON works_on.pno = project.pnumber
    GROUP BY 
        pname
    )
WHERE
    averageHours < (SELECT AVG(hours) FROM works_on);
    

-- 10.
SELECT UNIQUE
    fname, lname, COUNT(*) OVER(PARTITION BY e.ssn) AS numProjects
FROM 
    employee e INNER JOIN works_on w ON 
        e.ssn = w.essn;


