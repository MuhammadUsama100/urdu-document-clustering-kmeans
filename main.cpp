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
# include "kvector.cpp"
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
	map <long double, Kvector <DocData> >  dataCluster;
	Kvector <long double> elbowMethord; 
	map <long double, Kvector <DocData> >get_Clusterdata() {
		return dataCluster;
	}

	
	void kmeansClustering(map<string, Kvector<DocData>> doc_distances) {
		Kvector <DocData> data(doc_distances.size());
		map<string, Kvector<DocData>>  ::iterator index  = doc_distances.begin();
		cout << "areeb :"<<doc_distances.size() << endl ; 
		for (int i = 0;i < doc_distances.size(); i++) {
			long double  sum = FindSumOfVector(index->second);
			DocData docdata; 
			docdata.docDistance = sum; 
			docdata.DocPath = index->first;
			data[i] = docdata;
			index++;
		}
		
		//data = documentDistancing(data);

		int k = this->FindKByAlbowMethord(data); // there would be some changes 
		cout << endl;
		this->formGroupByCalculatingDistance(k, data); // working fine 

	}
	long double maxData(Kvector  <DocData> data) {
		vector  <long double>  tempDocDistance(data.getSize());
		for (int i = 0; i < data.getSize(); i++) {
			tempDocDistance[i] = data[i].docDistance;

		}
		//cout << "usama" <<*max_element(tempDocDistance.begin(), tempDocDistance.end());
		return  *max_element(tempDocDistance.begin(), tempDocDistance.end());

	}
	long double minData(Kvector  <DocData> data) {
		vector  <long double>  tempDocDistance(data.getSize());
		for (int i = 0; i < data.getSize(); i++) {
			tempDocDistance[i] = data[i].docDistance;
		}
		return  *min_element(tempDocDistance.begin(), tempDocDistance.end());

	}


	// check For all Possible  Clusters 
	long double  checkCluster(Kvector <long double> k, Kvector <DocData>  data) {
		long double  sum = 0;
		for (int i = 0; i < data.getSize(); i++) {
			long double distanceOfDataAndk = maxData(data);
			//cout << distanceOfDataAndk << " "; 
			for (int j = 0; j < k.getSize(); j++) {
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
	int  FindKByAlbowMethord(Kvector <DocData> data) {
		int NumClusters = data.getSize() / 2;

		Kvector <long double> clusters(NumClusters);
		srand(time(NULL));

		// random  point identification 
		Kvector <long double> numClustersK;
		for (int i = 0; i < NumClusters; i++) {
			numClustersK.pushBack(data[rand() % (data.getSize() - 1)].docDistance);
			clusters[i] = checkCluster(numClustersK, data);
			//cout  << clusters[i] << "  ";
		}

		// real value of k   means number of cluster that are formed in real chosen by try error 
		int realValueOfK = 1;


		// (f1 - f-1) / 2h
		cout << endl;


		for (int i = 0;i < clusters.getSize(); i++) cout << clusters[i] << endl;
		this->elbowMethord = clusters; 
		for (int i = 1; i < clusters.getSize(); i++) {
			if (fabsf(clusters[i] - clusters[i - 1]) > 500) realValueOfK++;
			else break;
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
		//realValueOfK = 6; //
		return realValueOfK;
	}

	//  measure the distances  , k  =  number of clusters 
	void formGroupByCalculatingDistance(int k, Kvector <DocData> data) {
		// min /  max value  
		long double minvalue = minData(data);
		long double maxvalue = maxData(data);
		cout << "Data points : " << minvalue << " " << maxvalue << endl;

		// random clusters 
		srand(time(NULL));
		map <long double, Kvector <DocData>  > clustersFormed;

		// centrioid vector 
		Kvector <long double>  centrioid(k);

		// random centroids vector  issue for size hence -1 is suqested 
		for (int i = 0; i < k;i++) {
			centrioid[i] = fmod(rand(), maxvalue) + minvalue;
			Kvector <DocData> vector;
			clustersFormed[centrioid[i]] = vector;
		}

		// calculating groups   	
		for (int i = 0;i < data.getSize(); i++) {
			long double  minDistance = 100000000000000000;
			long double selectedCentroid = 0;
			for (int j = 0; j < centrioid.getSize(); j++) {
				if (fabsf(data[i].docDistance - centrioid[j]) < minDistance) {
					minDistance = fabsf(data[i].docDistance - centrioid[j]);
					selectedCentroid = centrioid[j];
				}
			}
			clustersFormed[selectedCentroid].pushBack(data[i]);

		}
		cout <<"size  : " <<clustersFormed.size();
		clustersFormed = repositionCentroid(clustersFormed);
		cout << "size  : " << clustersFormed.size();
		clustersFormed = reGrouping(clustersFormed);
		cout << "size  : " << clustersFormed.size();
		convergence(clustersFormed);
		cout << "size  : " << clustersFormed.size();
		map <long double, Kvector <DocData> >  ::iterator  index = clustersFormed.begin();
		index = clustersFormed.begin();

		while (index != clustersFormed.end()) {
			cout << endl << "index" << " " << index->first << endl << endl << "value ";
			for (int i = 0; i < index->second.getSize(); i++) cout << " {" << index->second[i].DocPath << " " << index->second[i].docDistance << " }";
			cout << endl;
			index++;

		}
		this->dataCluster = clustersFormed;

	}
	long double FindSumOfVector(Kvector <DocData>  clustersFormed) {
		vector  <long double>  tempDocDistance(clustersFormed.getSize());
		for (int i = 0; i < clustersFormed.getSize(); i++) {
			tempDocDistance[i] = clustersFormed[i].docDistance;
		}
		return accumulate(tempDocDistance.begin(), tempDocDistance.end(), 0.00);
	}

	bool checkIfNewGroupingDone(map  <long double, Kvector <DocData> > clustersFormed) {

		map <long double, Kvector <DocData> >  ::iterator  index = clustersFormed.begin();
		while (index != clustersFormed.end()) {
			long double average = FindSumOfVector(index->second) / index->second.getSize();
			if (fabsf(index->first - average) > 0.0000000000000000000000001) return  true;
			index++;
		}
		return false;
	}


	// convergence
	void convergence(map  <long double, Kvector <DocData> > & clustersFormed) {
		while (checkIfNewGroupingDone(clustersFormed)) {
			cout << "usama1";
			clustersFormed = repositionCentroid(clustersFormed);
			clustersFormed = reGrouping(clustersFormed);
		}
		cout << "usama";
	}

	// there might be issue in float in line 126
	map <long double, Kvector <DocData> > reGrouping(map  <long double, Kvector <DocData> > &clustersFormed) {
		map <long double, Kvector <DocData>  > newclustersFormed;
		map <long double, Kvector <DocData> >  ::iterator  index = clustersFormed.begin();

		while (index != clustersFormed.end()) {
			
			for (int i = 0; i < index->second.getSize(); i++) {
				long double selectedCentroid = 0;
				long double minDistance = 100000000;
				
				map <long double, Kvector <DocData> >  ::iterator  count = clustersFormed.begin();
				while (count != clustersFormed.end()) {
					if (fabsf(index->second[i].docDistance - count->first) < minDistance) {
						minDistance = fabsf(index->second[i].docDistance - count->first);
						selectedCentroid = count->first;
					}
					count++;
				}

				newclustersFormed[selectedCentroid].pushBack(index->second[i]);
			}
			
			index++;
		}
		return newclustersFormed;
	}

	// Re calculation of Centroid
	map <long double, Kvector <DocData> >  repositionCentroid(map  <long double, Kvector <DocData> > &clustersFormed) {
		map <long double, Kvector <DocData> >  ::iterator  index = clustersFormed.begin();

		map <long double, Kvector <DocData> > newcluster ;
		while (index != clustersFormed.end()) {
			//check
			long double average = FindSumOfVector(index->second) / index->second.getSize();
			newcluster[average].insert(newcluster[average].end() ,  index->second.begin() , index->second.end());
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

	while (getline(file, line)) content += line;

	file.close();

	return content;
}

bool is_stop_word(wstring word) {
	wstring tags[] = { L"PU", L"PSP", L"PDM", L"PRP", L"PRT", L"PRD", L"PRR", L"PRS", L"CC", L"CD", L"SYM",L"AUXT", L"AUXM", L"AUXA", L"APNA", L"VBF", L"VALA", L"NEG", L"SC", L"SCP", L"RB" };
	for (wstring tag : tags) {
		if (word.find(tag) != string::npos) return true;
	}
	return false;
}

double get_max_tf(StringFrequencyMap& terms) {
	StringFrequencyMap::iterator max = max_element(terms.begin(), terms.end(),
		[](const pair<wstring, double>& p1, const pair<wstring, double>& p2) {
			return p1.second < p2.second;
		}
	);

	if (max != terms.end())return max->second;
	return 1;
}

Doc& generate_tfs(string doc_id, StringFrequencyMap& terms) {
	double max = get_max_tf(terms);
	for (StringFrequencyMap::iterator iter = terms.begin(); iter != terms.end(); iter++)
	{
		iter->second = iter->second / max;
	}

	Doc* doc = new Doc();
	doc->doc_id = doc_id;
	doc->terms = terms;

	return *doc;
}

void calculate_idfs(int n_docs, StringFrequencyMap& term_doc_frequencies) {
	for (StringFrequencyMap::iterator iter = term_doc_frequencies.begin(); iter != term_doc_frequencies.end(); iter++)
	{
		iter->second = log10(n_docs / (iter->second)); // inverse document frequency of each term
	}
}

void generate_tfidfs(vector<Doc>& docs, StringFrequencyMap& term_idfs) {
	wofstream tfidfs_output("tfidfs.txt");

	int i = 0;

	// Calculating tfidfs
	vector<Doc>::iterator docs_iter = docs.begin();
	while (docs_iter != docs.end()) { // Loop over all documents
		std::wstring wstr(docs_iter->doc_id.length(), L' ');
		copy(docs_iter->doc_id.begin(), docs_iter->doc_id.end(), wstr.begin());
		tfidfs_output << wstr << endl;

		StringFrequencyMap::iterator term_idf_bag_iter = term_idfs.begin();
		while (term_idf_bag_iter != term_idfs.end()) { // Loop over all terms
			docs_iter->terms[term_idf_bag_iter->first] *= term_idf_bag_iter->second; // tf*idf
			term_idf_bag_iter++;
		}

		StringFrequencyMap::iterator term_pointer = docs_iter->terms.begin();
		while (term_pointer != docs_iter->terms.end()) {
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

	for (pair<wstring, double> el : d1.terms) {
		res.push_back(pow(d1.terms[el.first] - d2.terms[el.first], 2));
	}

	return sqrt(accumulate(res.begin(), res.end(), 0.00));
}

int main() {
	ifstream folder_names_stream("dataset/foldernames.txt");

	string folder_name;
	vector<string> folder_names;

	// get folder names from dataset
	while (getline(folder_names_stream, folder_name)) {
		folder_names.insert(folder_names.end(), folder_name);
	}

	vector<Doc> docs;

	folder_names_stream.close();

	StringFrequencyMap term_idf_bag;
	int n_docs = 0;

	cout << "Processing files...\n";

	// iterate over each folder
	for (vector<string>::iterator folder_iter = folder_names.begin(); folder_iter != folder_names.end(); folder_iter++) {

		string dir_path = "dataset/" + *folder_iter;
		ifstream filenames_stream(dir_path + "/filenames.txt", ios::binary);
		string mstr;

		// iterate over each file in the folder
		while (getline(filenames_stream, mstr)) {
			n_docs++;
			string file_path = dir_path + "/" + mstr;
			wstring content = get_file_contents(file_path);

			StringFrequencyMap doc_tf_bag; // contains term frequencies 
			bool term_found_in_document = false;
			int n_words = 0;

			wstring::iterator content_iter = content.begin();

			wstring word;


			// loop over the content
			while (content_iter != content.end()) {
				wchar_t ch = *content_iter;
				if (ch != 32 && ch != 10 && ch != 13) word += ch; // keep adding character until space/enter/return found
				else if (word.length()) {
					if (is_stop_word(word)) { // if word is a stop word, ignore
						content_iter++;
						word = L"";
						continue;
					}

					doc_tf_bag[word]++; // increment count of word if found

					n_words++;

					if (!term_found_in_document) { // if word previously not found in document
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

	cout << "No of tederms: " << term_idf_bag.size() << endl;
	cout << "No of docs: " << docs.size() << endl;

	cout << "Calculating tfidfs...\n";

	generate_tfidfs(docs, term_idf_bag);

	// perform k-means on docs vector
	
	/*Kmeans  kmeans;
	kmeans.kmeansClustering(docs);

	map<long double, vector<DocData>> cluster = kmeans.dataCluster;
	int cluster_count = 0;
	map<long double, vector<DocData>>::iterator cluster_it = cluster.begin();
	while(cluster_it != cluster.end()) {
		int doc_count = 0;
		cout << "Cluster " << cluster_count++ << ": \n";
		vector<DocData>::iterator docdata_it = cluster_it->second.begin();
		while(docdata_it != cluster_it->second.end()) {(
			cout << doc_count++ << ": " << docdata_it->DocPath << "\n";
			docdata_it++;
		}
		cluster_it++;
	}*/

	cout << "Calculating inter document distances...\n" << endl;

	vector<Doc> test_docs(docs.begin(), docs.begin() + 100);

	map<string, Kvector<DocData>> doc_distances;
	Kvector<DocData> distances;
	for (Doc d : test_docs) {
		distances.clear();
		for (Doc d2 : test_docs) {
			DocData doc_data;
			doc_data.DocPath = d2.doc_id;
			doc_data.docDistance = doc_distance(d, d2);
			distances.pushBack(doc_data);
		}
		doc_distances[d.doc_id] = distances;
	}

	/*for (pair<string, vector<DocData>> p : doc_distances) {
		cout << p.first << ": " << endl;
		for (DocData doc_data : p.second) {
			cout << doc_data.DocPath << ": " << doc_data.docDistance << endl;
		}
	}
	*/
	cout << "Calculating Kmeans...\n" << endl;
	Kmeans  kmeans; 
	kmeans.kmeansClustering(doc_distances);
	//kmeans.elbowMethord;  to get curve that decides the number of clusters 
	//kmeans.dataCluster; to get clusters in data 

	
	

	cout << "Done.\n";

	return 0;
}