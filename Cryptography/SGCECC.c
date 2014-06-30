#include<stdio.h> 
struct pair		// Structure of a point on EC 
	{
		int x,y;	// X & Y co-ordinate of a point	
	};
struct pair keygen(int,struct pair);	// Transformation of a point on EC using a number : n(x,y)
struct pair trans_add(struct pair, struct pair);	// Transformation of a point on EC using another point : P + Q
struct pair trans_sub(struct pair, struct pair);	// P - Q
int p , a=0 , b=0;	// Prime no. p for Fp : EC equation : y^2 = x^3 + aX + b
main()
{
	int i,j,k=-1,g;	// i= participant number , k= total points on EC , g= Base point of EC
	printf("\n Initialization process ( Both user & chairperson ) \n ---------------------------------------------\n");
	do {
		printf("\n Choose a prime number : ");	// Prime no. to define Finite Field Fp
		scanf("%d",&p);
		j=0;
		for(i=2;i<p;i++)			// Prime checking
			if((p % i)==0)
				j=1;
	} while(j);
	struct pair ecc[2*p*p],rp,bp;	// Points on EC , Random point & Base point
	while(4*a*a*a + 27*b*b == 0)	// Singularity constraint on EC : 4a^3 + 27b^2 != 0
	{
		printf("\n\n Choose elliptic curve parameters a and b : ");
		scanf("%d %d",&a,&b);
	}
	for(i=0;i<p;i++)	// Generating points on EC
		for(j=0;j<p;j++)
			if( ((i*i*i+a*i+b) % p) == (j*j % p) ) // y^2 mod p = (x^3 + aX + b) mod p
			{
				ecc[++k].x=i;
				ecc[k].y=j;
				ecc[++k].x=i;
				if(j != 0)			// root of j^2 = +j & -j
					ecc[k].y=p-j;
				else
					ecc[k].y=j;
				break;
			}
	printf("\n\n Set of Sample Points on EC : ");	
	for(i=0;i<=k;i++)
		printf("\t ( %d , %d )",ecc[i].x,ecc[i].y);	// Points on EC within Finite Field Fp
	bp=ecc[0];
	for(j=0;j<=k;j++)
		if((ecc[j].x+ecc[j].y)<(bp.x+bp.y))	// Choosing Base point with smallest co-ordinate on EC
			bp=ecc[j];	
	printf("\n Base point G : ( %d , %d )\n",bp.x,bp.y);
	printf("\n Enter no. of participants : ");	// Total no of participants under chairPerson
	scanf("%d",&i);
	int par_pvt[i],chpr_pvt;	// Private key of ChairPerson & each Participant
	struct pair par_pub[i],chpr_pub,par_shared[i],encr_shared[i],decr_shared[i]; // Public keys, Shared keys
	printf("\n\n Chairperson site \n ---------------- \n");
	do {
		printf("\n Choose private key of chairperson ( 0<key<p ) : ");
		scanf("%d",&chpr_pvt);			// 0 < ChairPerson Private key < p
	} while((chpr_pvt >= p) || (chpr_pvt <= 0) );
	chpr_pub=keygen(chpr_pvt,bp);			// Generating Public key from Private key of ChairPerson
	printf("\n Public key of chairperson : ( %d , %d ) \n",chpr_pub.x,chpr_pub.y);
	printf("\n Participant site \n ---------------- \n");	
	for(j=0;j<i;j++)
	{
		do {
			printf("\n Choose private key of participant[%d] ( 0<key<p ) : ",j);
			scanf("%d",&par_pvt[j]);		// 0 < Participant Private key < p
		} while((par_pvt[j] >= p) || (par_pvt[j] <= 0) );	
		par_pub[j]=keygen(par_pvt[j],bp);	// Generating Pulic key from Private key of Participant
		printf("\n Public key of participant[%d] : ( %d , %d )",j,par_pub[j].x,par_pub[j].y);
		par_shared[j]=keygen(par_pvt[j],chpr_pub);	// Generating Shared key by each participant
		printf("\n Shared key of participant[%d] : ( %d , %d )",j,par_shared[j].x,par_shared[j].y);
		encr_shared[j]=trans_add(par_shared[j],keygen(par_pvt[j],chpr_pub));  // Encrypting Shared key by participant
		printf("\n Encrypted Shared key of participant[%d] : ( %d , %d ) \n",j,encr_shared[j].x,encr_shared[j].y);
	}	
	printf("\n Chairperson site \n ---------------- \n");
	struct pair combine_key,group_key,encr_group_key[i],decr_group_key[i];	// Combine & Group key
	while(1)
	{
		printf("\n Choose a random pair (any point on EC) : ");
		scanf("%d %d",&rp.x,&rp.y);
		for(j=0;j<=k;j++)
			if((rp.x==ecc[j].x)&&(rp.y==ecc[j].y))	// Random Point choosen by chairperson need to be on EC
				j=k+1;
		if(j==k+2)
			break;
	}
	printf("\n");
	combine_key=rp;		// combine key initialized with random point rp
	for(j=0;j<i;j++)
	{
		decr_shared[j]=trans_sub(encr_shared[j],keygen(chpr_pvt,par_pub[j]));//Decrypting Shared key from participant
		printf("\n Decrypted Shared key of participant[%d] : ( %d , %d )",j,decr_shared[j].x,decr_shared[j].y);
		combine_key=trans_add(combine_key,decr_shared[j]);		// Calculating Combine key
	}							
	printf("\n\n Combine key : ( %d , %d )\n",combine_key.x,combine_key.y);
	group_key=keygen(chpr_pvt,combine_key);				// Generating Group key from Combine key
	printf("\n Group key : ( %d , %d )\n",group_key.x,group_key.y);
	printf("\n Participant site \n ------------------- \n");
	for(j=0;j<i;j++)
	{
		encr_group_key[j]=trans_add(group_key,keygen(chpr_pvt,par_pub[j]));//Encrypted Groupkey to the participant
		printf("\n Encrypted Group Key to participant[%d] : ( %d , %d )",j,encr_group_key[j].x,encr_group_key[j].y);
		decr_group_key[j]=trans_sub(encr_group_key[j],keygen(par_pvt[j],chpr_pub));// Decryption to get Group key
		printf("\n Decrypted Group Key for participant[%d]:( %d , %d )\n",j,decr_group_key[j].x,decr_group_key[j].y);
	}
}						// Group Key ready to use

struct pair keygen(int number,struct pair point)	// Transformation of a point on EC using a number : n(x,y)
{
	int i;
	struct pair gen_point=point;			// if n=1 then return (x,y)
	if(number > 1)
		gen_point=trans_add(point,point);	// Doubling : P + P
	for(i=3;i<=number;i++)
		gen_point=trans_add(point,gen_point);	// Addition : (((((2P+P)+P)+P)+P)+P)+P.....
	return gen_point;
}

struct pair trans_sub(struct pair pointA, struct pair pointB)	// Point Subtraction in case of decryption
{
	pointB.y = (p - pointB.y) % p;			// for pointB : y = -y mod p
	return trans_add(pointA,pointB);		// call for Addition
}

struct pair trans_add(struct pair pointA, struct pair pointB)	// Transformation of a point using another/same point on EC 
{
	int s_numerator,s_denominator,s,s_modified_numerator,s_modified_quotient;
	struct pair result_point;
	if ((pointA.x==pointB.x)&&(pointA.y==pointB.y))		// Doubling
	{
		s_numerator= 3*pointA.x*pointA.x + a;		// s = (3x^2 + a)/2y mod p
		s_denominator= 2*pointA.y;
	}
	else							// Addition
	{
		s_numerator = pointA.y - pointB.y;		// s = (y1 - y2)/(x1 - x2) mod p 
		s_denominator = pointA.x - pointB.x;
		if((pointA.x == 0)&&(pointA.y == 0))
		{
			if((pointB.x == 0)&&(pointB.y == 0))	
				s_denominator=0;
			else
				return pointB;			// P + O (point at infinity) = P
		}
		else if((pointB.x == 0)&&(pointB.y == 0))
			return pointA;	
	}
	if(s_denominator==0)
	{
		result_point.x=0;		// if s_denominator = 0 : point at infinity (0,0)
		result_point.y=0;
		return result_point;
	}	
	if(s_numerator >= p)
		s_numerator = s_numerator % p;			// 0 <= s_numerator < p
	while(s_numerator < 0)
		s_numerator = s_numerator + p;
	s_modified_numerator=1;					// always s_modified_numerator = 1 initially
	while((s_modified_numerator % s_denominator) != 0)
		s_modified_numerator = p + s_modified_numerator;	// should be divisible by s_denominator
	s_modified_quotient = s_modified_numerator / s_denominator;
	if(s_modified_quotient >= p)
		s_modified_quotient = s_modified_quotient % p;		// 0 <= s_modified_quotient < p
	while(s_modified_quotient < 0)
		s_modified_quotient = s_modified_quotient + p;
	s=(s_numerator * s_modified_quotient) % p;		// 0 <= s < p
	result_point.x = s*s - pointA.x - pointB.x; 		// x = s^2 - x1 -x2
	if(result_point.x >= p)
		result_point.x = result_point.x % p;		// 0 <= x < p
	while(result_point.x < 0)
		result_point.x = result_point.x + p;		
	result_point.y= s*(pointA.x - result_point.x) - pointA.y;	// y = s( x1 - x) - y1
	if(result_point.y >= p)
		result_point.y = result_point.y % p;		// 0 <= y < p
	while(result_point.y < 0)
		result_point.y = result_point.y + p;
	return result_point;
}
