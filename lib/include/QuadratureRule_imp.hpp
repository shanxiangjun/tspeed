namespace{

    template<int N>
	void gauleg(Eigen::Matrix<double,N,1> &w, Eigen::Matrix<double,N,1> &x, double a, double b)
    {
	unsigned int n=N;
	double m=(n+1.)/2.0f;
	double xm=0.5*(b+a);
	double xl=0.5*(b-a);
	
	double z, z1, pi=M_PI, p1, p2, p3, pp;
	double eps = std::numeric_limits<double>::epsilon();
	//double eps = 1e-28;

	for(int i=1; i<=m; ++i){
	    z=cos(pi*(i-0.25)/(n+0.5));
	    do{
		p1=1.0;
		p2=0.0;
		for(int j=1; j<=n; ++j)
		{
		    p3=p2;
		    p2=p1;
		    p1=((2.0*j-1.0)*z*p2-(j-1.0)*p3)/j;
		}
		pp=n*(z*p1-p2)/(z*z-1.0);
		z1=z;
		z=z1-p1/pp;
	    }while(std::abs(z-z1)>eps);
	x(i-1)=xm-xl*z;
	x(n-i)=xm+xl*z;
	//std::cout << n-i << ": " << x(n-i) << std::endl;
	w(i-1)=2.0*xl/((1.0-z*z)*pp*pp);
	w(n-i)=w(i-1);
	//std::cout << "=============================="<<std::endl;
	//std::cout << i << std::endl;
	//std::cout << w << std::endl;
	}
	//x=xx;
	//w=ww;
    }

}
namespace Tspeed
{
    template<int N>
	inline Eigen::Matrix<double, N, 2>  QuadratureRule<N>::edge_nodes(int i)const
	{
	    typedef Eigen::Matrix<double, N, 2> Mat;
	    Mat n;
	    if(i==2)
	    {
		n.col(0) = M_node_1D;
		n.col(1) = Eigen::Matrix<double,N,1>::Zero();
		return n;
	    }
	    if(i==0)
	    {
		n.col(0) = Eigen::Matrix<double,N,1>::Ones() -M_node_1D;
		n.col(1) = M_node_1D;
		return n;
	    }
	    if(i==1)
	    {
		n.col(0) = Eigen::Matrix<double,N,1>::Zero();
		n.col(1) = Eigen::Matrix<double,N,1>::Ones()-M_node_1D;
		return n;
	    }
	    std::cerr << "Wrong edge number" << std::endl;
	    exit(1);
	}
    template<int N>
	Gauss<N>::Gauss():QuadratureRule<N>()
    {
	//Vec M_w_1D;
	//Vec M_node_1D;
	//Vec2 M_w_2D;
	//Mat M_node_2D;
	//size_t M_nqn_1D;
	gauleg<N>(this->M_w_1D, this->M_node_1D, 0, 1);
	this->M_nqn_1D = this->M_w_1D.size();

	Vec x,w;
	gauleg<N>(w,x,-1,1);
	for(int i=0; i<N; ++i)
	{
	    for(int j = 0; j<N; ++j)
	    {
		this->M_w_2D(i*N+j) = (1-x(i))*w(i)*w(j)/8.; 
		this->M_node_2D(i*N+j, 0) = (1+x(i))/2.;
		this->M_node_2D(i*N+j, 1) = (1-x(i))*(1+x(j))/4.;
	    }
	}
	//std::cout << this->M_node_2D << std::endl;
	//
    }
}
