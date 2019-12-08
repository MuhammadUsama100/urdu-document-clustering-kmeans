#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <stdlib.h>

using namespace std;

typedef map<wstring, double> StringFrequencyMap;

typedef struct Doc {
	string doc_id;
	StringFrequencyMap terms;
} Doc;

typedef struct DocData {
	long double docDistance;
	string DocPath;

} DocData;

//  kmeans Algo
class Kmeans {

public:
	map <long double, vector <DocData> >  dataCluster ;

	map <long double, vector <DocData> >get_Clusterdata() {
		return dataCluster;
	}
	// calculating distance from each doc 
	vector <DocData>  documentDistancing(vector <DocData> data) {
		for (int i = 0;i < data.size(); i++) {
			vector <long double> newData(data.size());
			for (int j = 0; j < data.size(); j++) {
				newData[j] = fabsf(data[i].docDistance - data[j].docDistance);
			}
			long double datavalue = accumulate(newData.begin(), newData.end(), 0.00) / newData.size();
			data[i].docDistance = fabsf(datavalue);
		}
		return data;

	}
	void kmeansClustering(vector<Doc> docs) {
		vector <DocData> data(docs.size());
		for (int i = 0; i < docs.size(); i++) {
			StringFrequencyMap::iterator index = docs[i].terms.begin();
			long double sum = 0;
			while (index != docs[i].terms.end()) {
				sum = sum + pow(index->second,2) ; // square of terms 
				index++;
			}
			//sum =sum / docs[i].terms.size();
			DocData docdata; 
			docdata.docDistance = fabsf(sum);
			docdata.DocPath = docs[i].doc_id;
			data[i] = (docdata);
		}

		data = documentDistancing(data);

		int k = this->FindKByAlbowMethord(data); // there would be some changes 
		cout << endl;
		this->formGroupByCalculatingDistance(k, data); // working fine 

	}
	long double maxData(vector  <DocData> data) {
		vector  <long double>  tempDocDistance(data.size());
		for (int i = 0; i < data.size(); i++) {
			tempDocDistance[i] = data[i].docDistance;

		}
		//cout << "usama" <<*max_element(tempDocDistance.begin(), tempDocDistance.end());
		return  *max_element(tempDocDistance.begin(), tempDocDistance.end());

	}
	long double minData(vector  <DocData> data) {
		vector  <long double>  tempDocDistance(data.size());
		for (int i = 0; i < data.size(); i++) {
			tempDocDistance[i] = data[i].docDistance;
		}
		return  *min_element(tempDocDistance.begin(), tempDocDistance.end());

	}


	// check For all Possible  Clusters 
	long double  checkCluster(vector <long double> k, vector <DocData>  data) {
		long double  sum = 0;
		for (int i = 0; i < data.size(); i++) {
			long double distanceOfDataAndk = maxData(data);
			//cout << distanceOfDataAndk << " "; 
			for (int j = 0; j < k.size(); j++) {
				if (distanceOfDataAndk > pow(fabsf(k[j] - data[i].docDistance), 2)) {
					distanceOfDataAndk = pow(fabsf(k[j] - data[i].docDistance), 2);
				}
			}
			sum = sum + distanceOfDataAndk;
			//cout << sum; 
		}
		return sum;
	}

	// check for right number of clusters 
	int  FindKByAlbowMethord(vector <DocData> data) {
		int NumClusters = data.size() / 2;

		vector <long double> clusters(NumClusters);
		srand(time(NULL));

		// random  point identification 
		vector <long double> numClustersK;
		for (int i = 0; i < NumClusters; i++) {
			numClustersK.push_back(data[rand() % (data.size() - 1)].docDistance);
			clusters[i] = checkCluster(numClustersK, data);
			//cout  << clusters[i] << "  ";
		}

		// real value of k   means number of cluster that are formed in real chosen by try error 
		int realValueOfK = 0;


		// (f1 - f-1) / 2h
		cout << endl;


		//for (int i = 0;i < clusters.size(); i++) cout << data[i].docDistance << endl;

		vector <long double> clustersK(clusters.size());
		for (int i = 1; i < clusters.size(); i++) {
			clustersK[i] = (clusters[i] - clusters[i - 1]) / 2;
		}

		//double maxvalue = *max_element(clustersK.begin(), clustersK.end());
		//cout << "max  : " << maxvalue <<endl  ;
		//for (int i = 0; i < clustersK.size(); i++) cout << clustersK[i] << endl;  

		//double changeinterval = clustersK[0] ;
		//for (int i = 1; i < clustersK.size(); i++) {
			//if (clustersK[i] == maxvalue)  realValueOfK = i;
		//	if (changeinterval / 1.5 <= clustersK[i])  realValueOfK ++;
		//	changeinterval = clustersK[i]; 

		//}

		cout << " K : " << realValueOfK;
		realValueOfK = 15; //
		return realValueOfK;
	}
	
	//  measure the distances  , k  =  number of clusters 
	void formGroupByCalculatingDistance(int k, vector <DocData> data) {
		// min /  max value  
		long double minvalue = minData(data);
		long double maxvalue = maxData(data);
		cout << "Data points : " << minvalue << " " << maxvalue << endl;

		// random clusters 
		srand(time(NULL));
		map <long double, vector <DocData>  > clustersFormed;

		// centrioid vector 
		vector <long double>  centrioid(k);

		// random centroids vector  issue for size hence -1 is suqested 
		for (int i = 0; i < centrioid.size();i++) {
			// check
			centrioid[i] = fmod(rand() , maxvalue)  + minvalue;
			//cout << centrioid[i]; 
			vector <DocData> vector;
			clustersFormed[centrioid[i]] = vector;
		}
	
		// calculating groups   	
		for (int i = 0;i < data.size(); i++) {
			long double  minDistance = data[0].docDistance;
			long double selectedCentroid = 0;
			for (int j = 0; j < centrioid.size(); j++) {
				if (fabsf(data[i].docDistance - centrioid[j]) < minDistance) {
					minDistance = fabsf(data[i].docDistance - centrioid[j]);
					selectedCentroid = centrioid[j];
				}
			}
			clustersFormed[selectedCentroid].push_back(data[i]);

		}


		//cout <<"First check : " << clustersFormed.size() << endl;
		clustersFormed = repositionCentroid(clustersFormed);
		//cout << "First check : " << clustersFormed.size() << endl;
		clustersFormed = reGrouping(clustersFormed);

		//cout <<"before : " <<clustersFormed.size();
		convergence(clustersFormed);
		map <long double, vector <DocData> >  ::iterator  index = clustersFormed.begin();
		index = clustersFormed.begin();

		while (index != clustersFormed.end()) {
			cout << endl  <<"index" << " " << index->first <<endl << endl << "value ";
			for (int i = 0; i < index->second.size(); i++) cout << " {" <<  index->second[i].DocPath << " "<< index->second[i].docDistance <<" }";
			cout << endl;
			index++;

		}
		this->dataCluster = clustersFormed;

	}
	long double FindSumOfVector(vector <DocData>  clustersFormed) {
		vector  <long double>  tempDocDistance(clustersFormed.size());
		for (int i = 0; i < clustersFormed.size(); i++) {
			tempDocDistance[i] = clustersFormed[i].docDistance;
		}
		return accumulate(tempDocDistance.begin(), tempDocDistance.end() , 0.00 );
	}

	bool checkIfNewGroupingDone(map  <long double, vector <DocData> > clustersFormed) {

		map <long double, vector <DocData> >  ::iterator  index = clustersFormed.begin();
		while (index != clustersFormed.end()) {
			long double average = FindSumOfVector(index->second) / index->second.size();
				//cout << average   << " " << index->first << endl  ;
			//cout << "check" << fabsf(index->first - average) << endl; 
			if (fabsf(index->first - average) > 0.0000000000000000000000001 ) return  true;
			index++;
		}
		return false;
	}


	// convergence
	void convergence(map  <long double, vector <DocData> > & clustersFormed) {
		
		while (checkIfNewGroupingDone(clustersFormed)) {
			cout << "usama1";
			clustersFormed = repositionCentroid(clustersFormed);
			clustersFormed = reGrouping(clustersFormed);
		}

		
		cout << "usama"; 
	}

	// there might be issue in float in line 126
	map <long double, vector <DocData> > reGrouping(map  <long double, vector <DocData> > clustersFormed) {
		map <long double, vector <DocData>  > newclustersFormed ;
		map <long double, vector <DocData> >  ::iterator  index = clustersFormed.begin();
		
		while (index != clustersFormed.end()) {
			for (int i = 0; i < index->second.size(); i++) {
				long double minDistance = 100000000;
				long double selectedCentroid = 0;
				map <long double, vector <DocData> >  ::iterator  count = clustersFormed.begin();
				while (count != clustersFormed.end()) {
					if (fabsf(index->second[i].docDistance - count->first) < minDistance) {
						minDistance = fabsf(index->second[i].docDistance - count->first);
						selectedCentroid = count->first;
					}
					count++;
				}

				newclustersFormed[selectedCentroid].push_back(index->second[i]);
			}
			index++;
		}
		return newclustersFormed;
	}

	// Re calculation of Centroid
	map <long double, vector <DocData> >  repositionCentroid(map  <long double, vector <DocData> > clustersFormed) {
		map <long double, vector <DocData> >  ::iterator  index = clustersFormed.begin();

		map <long double, vector <DocData> > newcluster;
		while (index != clustersFormed.end()) {
			//check
			double average = FindSumOfVector(index->second) / index->second.size(); 
			newcluster[average] = index->second;
			index++;
		}
		return newcluster;
	}


};

//  end Kmeans 

wstring get_file_contents(string file_path) {
	wifstream file(file_path, ios::binary);
			
	wstring content;
	wstring line;

	while(getline(file, line)) content += line;
			
	file.close();

	return content;
}

bool is_stop_word(wstring word) {
	wstring tags[] = {L"PU", L"PSP", L"PDM", L"PRP", L"PRT", L"PRD", L"PRR", L"PRS", L"CC", L"CD", L"SYM",L"AUXT", L"AUXM", L"AUXA", L"APNA", L"VBF", L"VALA", L"NEG", L"SC", L"SCP", L"RB"};
	for(wstring tag: tags) {
		if(word.find(tag) != string::npos) return true;
	}
	return false;
}

double get_max_tf(StringFrequencyMap& terms) {
	StringFrequencyMap::iterator max = max_element(terms.begin(), terms.end(), 
		[](const pair<wstring, double>& p1, const pair<wstring, double>& p2) {
			return p1.second < p2.second; 
		}
	);
	
	if(max != terms.end())return max->second;
	return 1;
}

Doc& generate_tfs(string doc_id, StringFrequencyMap& terms) {
	double max = get_max_tf(terms);
	for(StringFrequencyMap::iterator iter = terms.begin(); iter != terms.end(); iter++)
	{
		iter->second = iter->second / max;
	}

	Doc *doc = new Doc();
	doc->doc_id = doc_id; 
	doc->terms = terms;

	return *doc;
}

void calculate_idfs(int n_docs, StringFrequencyMap& term_doc_frequencies) {
	for(StringFrequencyMap::iterator iter = term_doc_frequencies.begin(); iter != term_doc_frequencies.end(); iter++)
	{
		iter->second = log10(n_docs / (iter->second)); // inverse document frequency of each term
	}
}

void generate_tfidfs(vector<Doc>& docs, StringFrequencyMap& term_idfs) {
	wofstream tfidfs_output("tfidfs.txt");

	int i = 0;
	
	// Calculating tfidfs
	vector<Doc>::iterator docs_iter = docs.begin();
	while(docs_iter != docs.end()) { // Loop over all documents
		std::wstring wstr(docs_iter->doc_id.length(), L' ');
		copy(docs_iter->doc_id.begin(), docs_iter->doc_id.end(), wstr.begin());
		tfidfs_output << wstr << endl;

		StringFrequencyMap::iterator term_idf_bag_iter = term_idfs.begin();
		while(term_idf_bag_iter != term_idfs.end()) { // Loop over all terms
			docs_iter->terms[term_idf_bag_iter->first] *= term_idf_bag_iter->second; // tf*idf
			term_idf_bag_iter++;
		}

		StringFrequencyMap::iterator term_pointer = docs_iter->terms.begin();
		while(term_pointer != docs_iter->terms.end()) {
			tfidfs_output << term_pointer->first << L": " << term_pointer->second << endl;	
			term_pointer++;
		}
		i++;
		docs_iter++;
	}

	tfidfs_output.close();
}

long double doc_distance(Doc d1, Doc d2) {
	vector<long double> res;

	for(pair<wstring, double> el: d1.terms) {
		res.push_back(pow(d1.terms[el.first] - d2.terms[el.first], 2));
	}

	return sqrt(accumulate(res.begin(), res.end(), 0.00));
}

int main() {
	ifstream folder_names_stream("dataset/foldernames.txt");
	
	string folder_name;
	vector<string> folder_names;
	
	// get folder names from dataset
	while(getline(folder_names_stream, folder_name)) {
		folder_names.insert(folder_names.end(), folder_name);
	}

	vector<Doc> docs;

	folder_names_stream.close();

	StringFrequencyMap term_idf_bag;
	int n_docs = 0;

	cout << "Processing files...\n";
	
	// iterate over each folder
	for(vector<string>::iterator folder_iter = folder_names.begin(); folder_iter != folder_names.end(); folder_iter++) {

		string dir_path = "dataset/" + *folder_iter;
		ifstream filenames_stream(dir_path + "/filenames.txt", ios::binary);
		string mstr;
		
		// iterate over each file in the folder
		while(getline(filenames_stream, mstr)) {
			n_docs++;
			string file_path = dir_path + "/" + mstr;
			wstring content = get_file_contents(file_path);
			
			StringFrequencyMap doc_tf_bag; // contains term frequencies 
			bool term_found_in_document = false;
			int n_words = 0;

			wstring::iterator content_iter = content.begin();
			
			wstring word;
			

			// loop over the content
			while(content_iter != content.end()) {
				wchar_t ch = *content_iter;
				if(ch != 32 && ch != 10 && ch != 13) word += ch; // keep adding character until space/enter/return found
				else if(word.length()) {
					if(is_stop_word(word)) { // if word is a stop word, ignore
						content_iter++;
						word = L"";
						continue;
					}

					doc_tf_bag[word]++; // increment count of word if found

					n_words++;
					
					if(!term_found_in_document) { // if word previously not found in document
						term_found_in_document = true; // set found word in this document flag to true

						term_idf_bag[word]++; // increment count if word found
					}
					word = L"";
				}
				content_iter++;
			}

			docs.insert(docs.end(), generate_tfs(file_path, doc_tf_bag));
		}
		filenames_stream.close();
	}
	folder_names_stream.close();

	cout << "Done.\n";
	
	calculate_idfs(n_docs, term_idf_bag);

	cout << "No of terms: " << term_idf_bag.size() << endl;
	cout << "No of docs: " << docs.size() << endl;

	cout << "Calculating tfidfs...\n";

	generate_tfidfs(docs, term_idf_bag);

	// perform k-means on docs vector
	//cout << "Calculating Kmeans...\n" << endl;
	/*Kmeans  kmeans;  
	kmeans.kmeansClustering(docs);
	
	map<long double, vector<DocData>> cluster = kmeans.dataCluster;

	int cluster_count = 0;
	map<long double, vector<DocData>>::iterator cluster_it = cluster.begin();
	while(cluster_it != cluster.end()) {
		int doc_count = 0;
		cout << "Cluster " << cluster_count++ << ": \n";
		vector<DocData>::iterator docdata_it = cluster_it->second.begin();
		while(docdata_it != cluster_it->second.end()) {
			cout << doc_count++ << ": " << docdata_it->DocPath << "\n";
			docdata_it++;
		}
		cluster_it++;
	}*/

	cout << "Calculating inter document distances...\n" << endl;

	vector<Doc> test_docs(docs.begin(), docs.begin()+70);

	map<string, vector<DocData>> doc_distances;
	vector<DocData> distances;
	for(Doc d: test_docs) {
		distances.clear();
		for(Doc d2: test_docs) {
			DocData doc_data;
			doc_data.DocPath = d2.doc_id;
			doc_data.docDistance = doc_distance(d, d2);
			distances.push_back(doc_data);
		}
		doc_distances[d.doc_id] = distances;
	}

	for(pair<string, vector<DocData>> p: doc_distances) {
		cout << p.first << ": " << endl;
		for(DocData doc_data: p.second) {
			cout << doc_data.DocPath << ": " << doc_data.docDistance << endl;
		}
	}
	

	cout << "Done.\n";

	return 0;
}
