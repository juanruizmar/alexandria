\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{manual}[Un estándar para artículos en negro sobre blanco]

\LoadClass[a4paper,12pt,fleqn]{article}

\RequirePackage{fancyhdr}
\RequirePackage{hyperref}
\RequirePackage{xcolor}
\RequirePackage{geometry}
\RequirePackage{titlesec}
\RequirePackage{tocloft}

\RequirePackage{amsmath}
\RequirePackage{amssymb}

\RequirePackage{gutenberg}

\RequirePackage[spanish]{babel}

\geometry{
    left=20mm,
    right=20mm,
    top=20mm,
    bottom=20mm,
}

\setlength{\parindent}{0pt}
\setlength{\parskip}{12pt}

\cftsetindents{section}{0em}{0em}
\cftsetindents{subsection}{0em}{0em}

\titleformat{\section}[block]{\color{\titlesColour}\normalfont\Large\bfseries}{}{20pt}{}[]
\titleformat{\subsection}[block]{\color{\titlesColour}\normalfont\large\bfseries}{}{20pt}{}

\renewcommand{\cftsecdotsep}{\cftdotsep}

\setcounter{secnumdepth}{0}\renewcommand{\maketitle}{
    \begin{center}
        {\color{\titlesColour}\LARGE \textbf{\@title}\par} \vskip 1em
        {\color{\titlesColour}\large \textbf{\@author}\par} \vskip 1em
        {\color{\titlesColour}\normalsize \@date\par}
    \end{center}
}

\newcommand{\setPageStyle}{      
    \fancyhead[L]{\color{\titlesColour}\textbf{\@title}}  
    \fancyhead[R]{\color{\titlesColour}\thepage}

    \renewcommand{\headrulewidth}{1.2pt}
    \renewcommand{\headrule}{\color{\decoratorColour}{\hrule height 1.2pt}}  

    \fancyfoot[C]{\color{\titlesColour}\@author}

    \renewcommand{\footrulewidth}{1.2pt}
    \renewcommand{\footrule}{\color{\decoratorColour}{\hrule height 1.2pt}}
}
