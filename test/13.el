(defvar test-input
  (let (l)
    (push '(1 1 3 1 1) l)
    (push '(1 1 5 1 1) l)
    (push '((1) (2 3 4)) l)
    (push '((1) 4) l)
    (push '(9) l)
    (push '((8 7 6)) l)
    (push '((4 4) 4 4) l)
    (push '((4 4) 4 4 4) l)
    (push '(7 7 7 7) l)
    (push '(7 7 7) l)
    (push '() l)
    (push '(3) l)
    (push '((())) l)
    (push '(()) l)
    (push '(1 (2 (3 (4 (5 6 7)))) 8 9) l)
    (push '(1 (2 (3 (4 (5 6 0)))) 8 9) l)
    (reverse l)))