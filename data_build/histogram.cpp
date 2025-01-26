#include "histogram.hpp"

inline void histogram::display(std::ostream &os){
    for(int i=0; i<n_segments(); ++i) os << frequencies_[i] << " " << min_+i*segment_length() << " " << max_-(n_segments()-i-1)*segment_length() << std::endl;
}
