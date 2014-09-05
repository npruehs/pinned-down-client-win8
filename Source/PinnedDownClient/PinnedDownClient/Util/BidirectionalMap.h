#pragma once

#include <map>

namespace PinnedDownClient
{
	namespace Util
	{
		template <typename TKey, typename TValue>
		class BidirectionalMap
		{
		public:
			void Add(TKey key, TValue value);
			TKey GetKeyOrDefault(TValue value, TKey defaultKey);
			TValue GetValueOrDefault(TKey key, TValue defaultValue);

		private:
			std::map<TKey, TValue> keyToValue;
			std::map<TValue, TKey> valueToKey;
		};

		template <typename TKey, typename TValue>
		void BidirectionalMap<TKey, TValue>::Add(TKey key, TValue value)
		{
			this->keyToValue.insert(std::pair<TKey, TValue>(key, value));
			this->valueToKey.insert(std::pair<TValue, TKey>(value, key));
		}

		template <typename TKey, typename TValue>
		TKey BidirectionalMap<TKey, TValue>::GetKeyOrDefault(TValue value, TKey defaultKey)
		{
			auto iterator = this->valueToKey.find(value);
			return iterator != this->valueToKey.end() ? iterator->second : defaultKey;
		}

		template <typename TKey, typename TValue>
		TValue BidirectionalMap<TKey, TValue>::GetValueOrDefault(TKey key, TValue defaultValue)
		{
			auto iterator = this->keyToValue.find(key);
			return iterator != this->keyToValue.end() ? iterator->second : defaultValue;
		}
	}
}
