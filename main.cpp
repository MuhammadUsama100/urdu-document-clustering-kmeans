#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

typedef map<wstring, double> StringFrequencyMap;

typedef struct Doc {
	string doc_id;
	StringFrequencyMap terms;
} Doc;

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

	cout << "Done.\n";

	return 0;
}