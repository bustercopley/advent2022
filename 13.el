;; -*- lexical-binding: t; -*-

(load-file "test/13.el")
(load-file "input/13.el")

(defun compare (a b)
  "Return 1 if A is before B, or -1 if B is before A, else nil."
  (cond ((and (integerp a) (integerp b))
         (cond ((< a b) 1)
               ((> a b) -1)))
        ((integerp a) (compare (list a) b))
        ((integerp b) (compare a (list b)))
        ((and (null a) (null b)) nil)
        ((null a) 1)
        ((null b) -1)
        (t (or (compare (car a) (car b)) (compare (cdr a) (cdr b))))))

(defun lessp (a b)
  (eq 1 (compare a b)))

(defun part1 (input)
  (let ((index 1) (sum 0))
    (while input
      (when (lessp (nth 0 input) (nth 1 input))
        (setq sum (+ sum index)))
      (setq input (cddr input)
            index (1+ index)))
    sum))

(defun part2 (input)
  (let ((l (cl-copy-list input)))
    (push '((2)) l)
    (push '((6)) l)
    (sort l #'lessp)
    (* (1+ (seq-position l '((2)))) (1+ (seq-position l '((6)))))))

(defun day-13 ()
  (interactive)
  (message "Day 13, Part 1, test: %s
Day 13, Part 1, real: %s
Day 13, Part 2, test: %s
Day 13, Part 2, real: %s\n"
           (part1 test-input)
           (part1 real-input)
           (part2 test-input)
           (part2 real-input)))
